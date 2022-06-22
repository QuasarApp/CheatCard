//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv1-5.h"
#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"
#include <CheatCard/api/api0/session.h>
#include "CheatCard/api/api0/user.h"

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscards.h>
#include <CheatCard/api/api1/card.h>
#include <CheatCard/api/api1/changeuserscards.h>

#include "CheatCard/nodeinfo.h"

#include <dbobjectsrequest.h>

#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include "CheatCard/basenode.h"


namespace RC {

ApiV1_5::ApiV1_5(BaseNode *node): ApiV1(node) {

}

int ApiV1_5::version() const {
    return 2;
}

QH::ParserResult ApiV1_5::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                       const QH::Header &pkgHeader,
                                       const QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::Session>(this, &ApiV1_5::processSession,
                                               pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardStatusRequest>(this, &ApiV1_5::processCardStatusRequest,
                                                    pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<APIv1::UsersCards>>(this, &ApiV1_5::processCardStatus,
                                                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardDataRequest>(this, &ApiV1_5::processCardRequest,
                                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<APIv1::Card>>(this, &ApiV1_5::processCardData,
                                                            pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1::RestoreDataRequest>(this,
                                                       &ApiV1_5::processRestoreDataRequest,
                                                       pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1::ChangeUsersCards>(this,
                                                     &ApiV1_5::processChanges,
                                                     pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV1_5::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    API::CardStatusRequest request;
    request.setSessionId(userSession);

    node()->sendData(&request, dist);
}

IAPIObjectsFactory *ApiV1_5::initObjectFactory() const {
    return new APIObjectsFactoryV1(db());
}

bool ApiV1_5::processCardStatus(const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards> > &cardStatuses,
                                const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV1::processCardStatus(cardStatuses, sender, pkg);
}

unsigned int ApiV1_5::processCardStatusBase(const QSharedPointer<APIv1::UsersCards> &cardStatus,
                                            const QByteArray& userSecreet,
                                            const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    auto dbCard = objectFactoryInstance()->getCard(cardStatus->getCard());
    auto dbUsersCards = objectFactoryInstance()->getUserCardData(
                cardStatus->getUser(),
                cardStatus->getCard());

    // ignore seels statuses that has a depricated time.
    if (dbUsersCards && dbUsersCards->getTime() > cardStatus->getTime()) {
        return 0;
    }


    if (!cardValidation(dbCard, userSecreet)) {

        QuasarAppUtils::Params::log("Receive not signed cards seal");
        return 0;
    }

    // Disable alert if this packge is ansver to restore request
    if (!applayPurchases(cardStatus, sender, pkg.triggerHash != _restoreDataPacakgeHash)) {
        return 0;
    }

    bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

    if (!dbCard || hasUpdate) {
        return cardStatus->getCard();
    }

    return 0;

}

bool ApiV1_5::processCardRequest(const QSharedPointer<API::CardDataRequest> &cardrequest,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardRequest(cardrequest, sender, pkg);

}

bool ApiV1_5::processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card>> &cards,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardData(cards, sender, pkg);

}

bool ApiV1_5::processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                        const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV1::processRestoreDataRequest(cardrequest, sender, pkg);
}

bool ApiV1_5::processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &cardStatus,
                                       const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV1::processCardStatusRequest(cardStatus, sender, pkg);

}

bool ApiV1_5::processChanges(const QSharedPointer<APIv1::ChangeUsersCards> &message,
                             const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!message->isValid()) {
        return false;
    }

    message->setPrintError(false);
    db()->insertObject(message);

    auto dbUsersCards = objectFactoryInstance()->getUserCardData(
                message->getUser(),
                message->getCard());

    if (!dbUsersCards) {
        dbUsersCards =  QSharedPointer<API::UsersCards>::create(message->getUser(), message->getCard());
    }

    dbUsersCards->setPurchasesNumber(dbUsersCards->getPurchasesNumber() + message->purchase());
    dbUsersCards->receive(message->receive());


    unsigned int requiredCard = processCardStatusBase(dbUsersCards.staticCast<APIv1::UsersCards>(),
                                                      message->secret(), sender, hdr);

    if (requiredCard) {
        API::CardDataRequest request;
        request.setCardIds({requiredCard});
        return node()->sendData(&request, sender, &hdr);
    }

    auto request = QSharedPointer<APIv1::RestoreDataRequest>::create();
    request->setUserKey(API::User::makeKey(message->secret()));

    return processRestoreDataRequest(request, sender, hdr);

}

bool ApiV1_5::processSession(const QSharedPointer<API::Session> &session,
                             const QH::AbstractNodeInfo *sender,
                             const QH::Header &pkg ) {

    return ApiV1::processSession(session, sender, pkg);
}

void ApiV1_5::restoreOldDateRequest(const QByteArray &curentUserKey,
                                    QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOldDateRequest(curentUserKey, dist);
}

void ApiV1_5::restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOneCardRequest(cardId, dist);

}


}
