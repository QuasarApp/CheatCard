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
#include <CheatCard/api/api1-5/cardupdated.h>
#include <CheatCard/api/api1-5/changeuserscards.h>
#include <CheatCard/api/api1-5/restoreresponce.h>
#include <CheatCard/api/api1-5/restoreresponce.h>
#include <CheatCard/api/api1-5/statusafterchanges.h>
#include <CheatCard/api/api1-5/updatecontactdata.h>
#include <CheatCard/api/api1-5/updatecontactdataresponce.h>

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

    result = commandHandler<QH::PKG::DataPack<APIv1::UsersCards>>(this,
                                                                  &ApiV1_5::processCardStatus,
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

    result = commandHandler<APIv1_5::ChangeUsersCards>(this,
                                                       &ApiV1_5::processChanges,
                                                       pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1_5::StatusAfterChanges>(this,
                                                         &ApiV1_5::processStatusAfterChanged,
                                                         pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1_5::CardUpdated>(this,
                                                  &ApiV1_5::processCardUpdate,
                                                  pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    result = commandHandler<APIv1_5::UpdateContactData>(this,
                                                  &ApiV1_5::processContacts,
                                                  pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    result = commandHandler<APIv1_5::UpdateContactDataResponce>(this,
                                                  &ApiV1_5::processContactsResponce,
                                                  pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1_5::RestoreResponce>(this,
                                                  &ApiV1_5::processRestoreResponce,
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

void ApiV1_5::processCardStatusWithoutCardRequests(
        const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards> > &cardStatuses) {

    for (const auto& cardStatus : cardStatuses->packData()) {
        auto dbCard = objectFactoryInstance()->getCard(cardStatus->getCard());
        auto dbUsersCards = objectFactoryInstance()->getUserCardData(
                    cardStatus->getUser(),
                    cardStatus->getCard());

        // ignore seels statuses that has a depricated time.
        if (dbUsersCards && dbUsersCards->getTime() > cardStatus->getTime()) {
            continue;
        }

        if (!cardValidation(dbCard, cardStatuses->customData())) {

            QuasarAppUtils::Params::log("Receive not signed cards seal");
            break;
        }

        // Disable alert if this packge is ansver to restore request
        if (!applayPurchases(cardStatus, nullptr, false)) {
            break;
        }
    }

    return;
}

bool ApiV1_5::processCardStatusBase(const QSharedPointer<APIv1::UsersCards> &cardStatus,
                                    const QByteArray& userSecreet,
                                    const QH::AbstractNodeInfo *sender,
                                    const QH::Header &pkg,
                                    unsigned int& neededCardId) {

    auto dbCard = objectFactoryInstance()->getCard(cardStatus->getCard());
    auto dbUsersCards = objectFactoryInstance()->getUserCardData(
                cardStatus->getUser(),
                cardStatus->getCard());

    // ignore seels statuses that has a depricated time.
    if (dbUsersCards && dbUsersCards->getTime() > cardStatus->getTime()) {
        return true;
    }

    if (!cardValidation(dbCard, userSecreet)) {

        QuasarAppUtils::Params::log("Receive not signed cards seal");
        return false;
    }

    // Disable alert if this packge is ansver to restore request
    if (!applayPurchases(cardStatus, sender, pkg.triggerHash != _restoreDataPacakgeHash)) {
        return false;
    }

    bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

    if (!dbCard || hasUpdate) {
        neededCardId = cardStatus->getCard();
    }

    return true;

}

bool ApiV1_5::processCardRequest(const QSharedPointer<API::CardDataRequest> &cardrequest,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardRequest(cardrequest, sender, pkg);

}

bool ApiV1_5::processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card>> &cards,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardData(cards, sender, pkg);

}

bool ApiV1_5::processCardUpdate(const QSharedPointer<APIv1_5::CardUpdated> &cardrequest,
                                const QH::AbstractNodeInfo *sender,
                                const QH::Header &hdr) {

    auto dbCard = objectFactoryInstance()->getCard(cardrequest->cardId());
    API::CardDataRequest request;

    if (!dbCard || dbCard->getCardVersion() < cardrequest->cardVersion()) {
        request.push(cardrequest->cardId());
    }

    if (request.getCardIds().size()) {
        return node()->sendData(&request, sender, &hdr);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1_5::processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                        const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    APIv1_5::RestoreResponce responce;
    responce.setUserKey(cardrequest->userKey());
    QH::PKG::DataPack<APIv1::UsersCards> usersData;

    collectDataOfuser(cardrequest->userKey(), usersData);
    responce.setUsersCards(usersData);

    auto contacts = node()->getSlaveKeys(cardrequest->userKey());
    contacts += node()->getMasterKeys(cardrequest->userKey());
    responce.setContacts(contacts);

    if (responce.isValid()) {
        return node()->sendData(&responce, sender, &hdr);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1_5::processStatusAfterChanged(const QSharedPointer<APIv1_5::StatusAfterChanges> &status,
                                        const QH::AbstractNodeInfo * sender,
                                        const QH::Header & hdr) {

    auto cardStatus = QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards>>::create(status->getLastStatus());

    if (status->neededCard()) {
        processCardStatusWithoutCardRequests(cardStatus);

        auto card = objectFactoryInstance()->getCard(status->getCard()).staticCast<APIv1::Card>();
        QH::PKG::DataPack<APIv1::Card> responce;
        responce.push(card);

        return node()->sendData(&responce, sender, &hdr);
    }

    if (!processCardStatus(cardStatus, sender, hdr)) {
        return false;
    }

    emit node()->sigSessionStatusResult(status.staticCast<API::Session>(), status->status());
    return true;
}

void RC::ApiV1_5::applayUpdateContactData(
        const QSharedPointer<RC::APIv1_5::UpdateContactData> &data,
                                          bool success) {
    if (success) {
        if (data->getRemove()) {
            db()->deleteObject(data);
        } else {
            db()->insertIfExistsUpdateObject(data);
        }
    }

    emit node()->sigContactsStatusResult(data.staticCast<API::Contacts>(),
                                         success, data->getRemove());
}

void ApiV1_5::processContactsResponcePrivate(unsigned int requestId, bool result) {

    if (auto object = _waitResponce.value(requestId)) {
        applayUpdateContactData(object, result);
        _waitResponce.remove(requestId);
    }
}

QH::PKG::DataPack<APIv1::UsersCards>
ApiV1_5::lastUserStatus(unsigned int cardId) {
    QH::PKG::DataPack<APIv1::UsersCards> responce;

    auto result = objectFactoryInstance()->getAllUserFromCard(cardId);

    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(node()->getCardVersion(cardId));
        responce.push(data.staticCast<APIv1::UsersCards>());
    }

    return responce;
}

bool ApiV1_5::processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &cardStatus,
                                       const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV1::processCardStatusRequest(cardStatus, sender, pkg);

}

bool ApiV1_5::processChanges(const QSharedPointer<APIv1_5::ChangeUsersCards> &message,
                             const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!message->isValid()) {
        return false;
    }

    auto dbUsersCards = objectFactoryInstance()->getUserCardData(
                message->getUser(),
                message->getCard());

    APIv1_5::StatusAfterChanges status;
    status.setSessionId(message->getSessionId());
    status.setUsercardId(message->getUsercardId());

    if (dbUsersCards) {
        int availabelFreeItems = node()->getFreeItemsCount(dbUsersCards);
        if (availabelFreeItems < message->receive()) {
            // return status false and new users statuses of this seller

            auto lastStatus = objectFactoryInstance()->
                    getUserCardData(message->getUser(), message->getCard());

            status.setStatus(false);
            status.addLastStatus(lastStatus.staticCast<APIv1::UsersCards>());
            if (!node()->sendData(&status, sender, &hdr)){
                return false;
            }
        }

    } else {
        dbUsersCards =  QSharedPointer<API::UsersCards>::create(message->getUser(), message->getCard());
    }

    message->setPrintError(false);
    db()->insertObject(message);

    dbUsersCards->setPurchasesNumber(dbUsersCards->getPurchasesNumber() + message->purchase());
    dbUsersCards->receive(message->receive());

    unsigned int neededCardId = 0;
    if (!processCardStatusBase(dbUsersCards.staticCast<APIv1::UsersCards>(),
                               message->secret(), sender, hdr, neededCardId)) {
        return false;
    }

    status.setNeededCard(neededCardId);

    auto lastStatus = objectFactoryInstance()->
            getUserCardData(message->getUser(), message->getCard());
    status.addLastStatus(lastStatus.staticCast<APIv1::UsersCards>());
    status.setStatus(true);

    return node()->sendData(&status, sender, &hdr);
}

bool ApiV1_5::processSession(const QSharedPointer<API::Session> &session,
                             const QH::AbstractNodeInfo *sender,
                             const QH::Header &pkg ) {

    return ApiV1::processSession(session, sender, pkg);
}

bool ApiV1_5::processRestoreResponce(const QSharedPointer<APIv1_5::RestoreResponce> &message,
                                     const QH::AbstractNodeInfo *sender,
                                     const QH::Header &hdr) {

    db()->doQuery(QString("DELETE FROM Contacts WHERE childUserKey= '%0'").
                  arg(QString(message->userKey().toBase64(QByteArray::Base64UrlEncoding))));


    for (const auto& contact: message->contacts().packData()) {
        db()->insertIfExistsUpdateObject(contact);
    }

    emit node()->sigContactsListChanged();

    return ApiV1::processCardStatusImpl(message->usersCards(), sender, hdr);
}

bool ApiV1_5::cardValidation(const QSharedPointer<API::Card> &cardFromDB,
                             const QByteArray &ownerSecret) const {
    switch (NodeTypeHelper::getBaseType(node()->nodeType())) {
    case NodeType::Server: {
        if (!cardFromDB)
            return true;

        auto signature = cardFromDB->ownerSignature();

        if (signature.isEmpty()) {
            return true;
        }

        auto ownerSignature =  API::User::makeKey(ownerSecret);

        if (signature == ownerSignature)
            return true;

        // check additional access
        auto contact = node()->getContactFromChildId(signature, ownerSignature);
        return contact;
    }

    default: {};
    }

    return true;
}

bool ApiV1_5::processContacts(const QSharedPointer<APIv1_5::UpdateContactData> &message,
                              const QH::AbstractNodeInfo * sender,
                              const QH::Header &hdr) {

    auto userKey = API::User::makeKey(message->userSecreet());
    auto userId = API::User::makeId(userKey);

    if (userId != message->getUser()) {
        QuasarAppUtils::Params::log("User try change permission for another user.",
                                    QuasarAppUtils::Error);
        return false;
    }

    if (message->getRemove()) {
        if (!db()->deleteObject(message.staticCast<API::Contacts>())) {
            QuasarAppUtils::Params::log("Fail to detele user permisiion",
                                        QuasarAppUtils::Error);
            return false;
        }

    } else {
        if (!db()->insertIfExistsUpdateObject(message.staticCast<API::Contacts>())) {
            QuasarAppUtils::Params::log("Fail to save user permisiion",
                                        QuasarAppUtils::Error);
            return false;
        }
    }

    APIv1_5::UpdateContactDataResponce responce;
    responce.setSuccessful(true);

    return node()->sendData(&responce, sender, &hdr);
}

bool ApiV1_5::processContactsResponce(
        const QSharedPointer<APIv1_5::UpdateContactDataResponce> &message,
        const QH::AbstractNodeInfo *sender,
        const QH::Header & hdr) {

    processContactsResponcePrivate(hdr.triggerHash, message->getSuccessful());
    return node()->removeNode(sender->networkAddress());
}

void ApiV1_5::restoreOldDateRequest(const QByteArray &curentUserKey,
                                    QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOldDateRequest(curentUserKey, dist);
}

void ApiV1_5::restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOneCardRequest(cardId, dist);

}

bool ApiV1_5::sendContacts(const API::Contacts &contact,
                           const QByteArray& secreet,
                           bool removeRequest,
                           QH::AbstractNodeInfo *dist) {


    auto request = QSharedPointer<RC::APIv1_5::UpdateContactData>::create(contact);
    request->setRemove(removeRequest);
    request->setUserSecreet(secreet);

    unsigned int pkgHash = node()->sendData(request.data(), dist);

    if (!pkgHash) {
        return false;
    }

    _waitResponce[pkgHash] = request;

    QTimer::singleShot(10000, nullptr, [this, pkgHash]() {
        processContactsResponcePrivate(pkgHash, false);
    });

    return true;
}

bool ApiV1_5::sendUpdateCard(unsigned int cardId,
                             unsigned int version,
                             QH::AbstractNodeInfo *dist) {
    APIv1_5::CardUpdated request;
    request.setCardVersion(version);
    request.setCardId(cardId);

    return node()->sendData(&request, dist, nullptr);

}


}
