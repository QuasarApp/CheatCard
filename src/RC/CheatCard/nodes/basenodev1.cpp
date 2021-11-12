//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenodev1.h"
#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "CheatCard/nodeinfo.h"
#include "CheatCard/user.h"

#include <CheatCard/card.h>
#include <dbobjectsrequest.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

namespace RC {

BaseNodeV1::BaseNodeV1(QH::ISqlDBCache *db): BaseNode(db) {

    useSystemSslConfiguration();
    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                       << QSslError::SelfSignedCertificateInChain
                       << QSslError::HostNameMismatch);
}

QH::ParserResult BaseNodeV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                          const QH::Header &pkgHeader,
                                          const QH::AbstractNodeInfo *sender) {

    auto parentResult = BaseNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    return QH::ParserResult::NotProcessed;
}

bool BaseNodeV1::cardValidation(const QSharedPointer<Card> &card,
                                const QByteArray& ownerSecret) const {
    Q_UNUSED(card)
    Q_UNUSED(ownerSecret)
    return true;
}

bool BaseNodeV1::sealValidation(const QSharedPointer<UsersCards> &userCardData,
                                const QSharedPointer<Card> &cardFromDb,
                                const QByteArray& ownerSecret) const {
    Q_UNUSED(userCardData)
    Q_UNUSED(cardFromDb)
    Q_UNUSED(ownerSecret)
    return true;
}

bool BaseNodeV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &) {

    CardDataRequest request;

    for (const auto& cardStatus : cardStatuses->packData()) {
        Card userrquest;
        userrquest.setId(cardStatus->getCard());

        auto dbCard = db()->getObject(userrquest);

        if (sealValidation(cardStatus, dbCard, cardStatuses->customData())) {

            QuasarAppUtils::Params::log("Receive not signed cards seal");
            break;
        }

        if (!applayPurchases(cardStatus, sender)) {
            break;
        }

        bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (!dbCard || hasUpdate) {
            request.push(cardStatus->getCard());
        }
    }

    if (request.getCardIds().size()) {

        if (!sendData(&request, sender)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return removeNode(sender->networkAddress());
}

bool BaseNodeV1::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                                 const QH::AbstractNodeInfo * sender) {

    return BaseNode::applayPurchases(dbCard, sender);
}

bool BaseNodeV1::processCardRequest(const QSharedPointer<CardDataRequest> &cardrequest,
                                    const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QH::PKG::DataPack<Card> cards{};

    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = getCard(cardId);

        if (!card) {
            QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                        arg(cardId),
                                        QuasarAppUtils::Error);
            continue;
        }
        cards.push(card);
    }

    if (!cards.packData().size()) {
        QuasarAppUtils::Params::log(QString("Failed to find any cards "),
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!sendData(&cards, sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool BaseNodeV1::processCardData(const QSharedPointer<QH::PKG::DataPack<Card>> &cards,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &) {

    if (!(cards && db())) {
        return false;
    }

    for (const auto &card: qAsConst(cards->packData())) {
        if (!card->isValid()) {
            QuasarAppUtils::Params::log("Received invalid card data!",
                                        QuasarAppUtils::Error);
            continue;
        }

        if (cardValidation(card, cards->customData())) {

            QuasarAppUtils::Params::log("Receive not signed card");
            break;
        }

        if (!db()->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit sigCardReceived(card);
    }

    return removeNode(sender->networkAddress());
}

bool BaseNodeV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &cardStatus,
                                          const QH::AbstractNodeInfo *sender, const QH::Header &) {

    auto sessionId = cardStatus->getSessionId();

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards", where);

    auto result = db()->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<UsersCards> responce;

    for (const auto &data : qAsConst(result->data())) {
        data->setCardVersion(getCardVersion(data->getCard()));
        responce.push(data);
    }

    if (!sendData(&responce, sender)) {
        return false;
    }

    return true;
}

bool BaseNodeV1::processSession(const QSharedPointer<Session> &session,
                                const QH::AbstractNodeInfo *sender,
                                const QH::Header &) {

    if (!session->isValid()) {
        return false;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    CardStatusRequest requestData;

    requestData.setSessionId(session->getSessionId());

    return sendData(&requestData, sender->networkAddress());

}


}
