//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "applicationversion.h"
#include "basenode.h"
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

BaseNode::BaseNode(QH::ISqlDBCache *db) {
    _db = db;

    registerPackageType<ApplicationVersion>();

    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                       << QSslError::SelfSignedCertificateInChain
                       << QSslError::HostNameMismatch);
}

QH::ParserResult BaseNode::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                        const QH::Header &pkgHeader,
                                        const QH::AbstractNodeInfo *sender) {

    auto parentResult = AbstractNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    // here node must be receive version of connected application.
    // if not use the default version (0)ApplicationVersion
    parentResult = commandHandler<ApplicationVersion>(this, &BaseNode::processAppVersion,
                                               pkg, sender, pkgHeader);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    int distVersion = static_cast<const NodeInfo*>(sender)->version();
    auto parser = _apiParsers.value(distVersion);


    if (!parser) {
        QuasarAppUtils::Params::log("Can't found requeried parser for version: " + QString::number(distVersion),
                                    QuasarAppUtils::Warning);

        return QH::ParserResult::NotProcessed;
    }

    return parser->parsePackage(pkg, pkgHeader, sender);
}

bool BaseNode::processAppVersion(const QSharedPointer<ApplicationVersion> &message,
                                 const QH::AbstractNodeInfo *sender,
                                 const QH::Header &) {

    auto nodeInfo = dynamic_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
    nodeInfo->setVersion(message->version());

    return true;
}

QH::AbstractNodeInfo *BaseNode::createNodeInfo(QAbstractSocket *socket,
                                               const QH::HostAddress *clientAddress) const {
    return new NodeInfo(socket, clientAddress);
}

void BaseNode::nodeConnected(QH::AbstractNodeInfo *node) {
    QH::AbstractNode::nodeConnected(node);

    ApplicationVersion appVersion;
    if (_apiParsers.size()) {
        int version = _apiParsers.last()->version();
        appVersion.setVersion(version);
    }

    sendData(&appVersion, node);
}

const QSharedPointer<User>& BaseNode::currentUser() const {
    return _currentUser;
}

void BaseNode::setCurrentUser(QSharedPointer<User> newCurrentUser) {
    _currentUser = newCurrentUser;
}

int BaseNode::getFreeItemsCount(unsigned int userId,
                                unsigned int cardId) const {
    return getFreeItemsCount(getUserCardData(userId, cardId));
}

int BaseNode::getFreeItemsCount(const QSharedPointer<UsersCards> &inputData) const {
    unsigned int freeIndex = getCardFreeIndex(inputData->getCard());
    return getFreeItemsCount(inputData, freeIndex);
}

int BaseNode::getFreeItemsCount(const QSharedPointer<UsersCards> &inputData,
                                unsigned int freeIndex) const {
    if (freeIndex <= 0)
        return 0;

    int freeItems = std::floor(inputData->getPurchasesNumber() /
                               static_cast<double>(freeIndex)) -
            inputData->getReceived();

    return freeItems;
}

int BaseNode::getCountOfReceivedItems(unsigned int userId,
                                      unsigned int cardId) {
    return getUserCardData(userId, cardId)->getReceived();
}

int RC::BaseNode::getCardFreeIndex(unsigned int cardId) const {
    QH::PKG::GetSingleValue request({"Cards", cardId}, "freeIndex");
    auto result = _db->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

unsigned int BaseNode::getCardVersion(unsigned int cardId) const {
    QH::PKG::GetSingleValue request({"Cards", cardId}, "cardVersion");
    auto result = _db->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

QString BaseNode::libVersion() {
    return CHEAT_CARD_VERSION;
}

QSharedPointer<User> BaseNode::getUserData(unsigned int userId) const {
    User request;
    request.setId(userId);

    return db()->getObject(request);
}

QSharedPointer<UsersCards>
BaseNode::getUserCardData(unsigned int userId, unsigned int cardId) const {
    UsersCards request;
    request.setCard(cardId);
    request.setUser(userId);

    auto purches = _db->getObject(request);
    return purches;
}

QList<QSharedPointer<UsersCards> >
BaseNode::getAllUserFromCard(unsigned int cardId, bool includeOwner) const {

    QString where = QString("card=%0").arg(cardId);

    if (!includeOwner) {
        where += " AND owner=0";
    }

    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                  where);

    return _db->getObject(request)->data();
}

QList<QSharedPointer<User> >
BaseNode::getAllUserDataFromCard(unsigned int cardId, bool includeOwner) const {
    QString where = QString("card=%0").arg(cardId);

    if (!includeOwner) {
        where += " AND owner=0";
    }

    where = QString("id IN (select user from UsersCards where %0)").arg(where);

    QH::PKG::DBObjectsRequest<User> request("Users",
                                            where);

    return _db->getObject(request)->data();
}

QSharedPointer<Card> BaseNode::getCard(unsigned int cardId) {
    Card request;
    request.setId(cardId);

    return _db->getObject(request);
}

QByteArray BaseNode::getUserSecret(unsigned int userId) const {
    QH::PKG::GetSingleValue reqest({"Users", userId}, "secret");

    auto result = db()->getObject(reqest);
    return result->value().toByteArray();
}

void RC::BaseNode::addApiParser(const QSharedPointer<iParser>& api) {
    _apiParsers[api->version()] = api;
}

QH::ISqlDBCache *BaseNode::db() const {
    return _db;
}

const QMap<int, QSharedPointer<QH::iParser> > &BaseNode::apiParsers() const {
    return _apiParsers;
}

void BaseNode::setApiParsers(const QMap<int, QSharedPointer<QH::iParser> > &newApiParsers) {
    _apiParsers = newApiParsers;
}


}
