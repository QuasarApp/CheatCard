//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "applicationversion.h"
#include "basenode.h"
#include "versionisreceived.h"
#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"
#include "CheatCard/api/api0/user.h"
#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userscards.h>

#include <CheatCard/api/api1/restoredatarequest.h>

#include "CheatCard/nodeinfo.h"
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

namespace RC {

BaseNode::BaseNode(QH::ISqlDBCache *db) {
    _db = db;

    registerPackageType<ApplicationVersion>();
    registerPackageType<VersionIsReceived>();


    registerPackageType<QH::PKG::DataPack<UsersCards>>();
    registerPackageType<QH::PKG::DataPack<Card>>();


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

    // here node must be receive responce that version is delivered.
    // when node receive this message then node status are confirmed
    parentResult = commandHandler<VersionIsReceived>(this, &BaseNode::versionDeliveredSuccessful,
                                                     pkg, sender, pkgHeader);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    auto distVersion = static_cast<const NodeInfo*>(sender)->version();
    auto parser = selectParser(static_cast<const NodeInfo*>(sender)->version());

    if (!parser) {
        QuasarAppUtils::Params::log(QString("Can't found requeried parser for versions: %0-%1").
                                    arg(distVersion.minimum()).
                                    arg(distVersion.maximum()),
                                    QuasarAppUtils::Warning);

        return QH::ParserResult::NotProcessed;
    }

    return parser->parsePackage(pkg, pkgHeader, sender);
}

bool BaseNode::processAppVersion(const QSharedPointer<ApplicationVersion> &message,
                                 const QH::AbstractNodeInfo *sender,
                                 const QH::Header &) {

    auto nodeInfo = dynamic_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
    nodeInfo->setVersion(*message.data());

    VersionIsReceived result;
    return sendData(&result, sender);
}

bool BaseNode::versionDeliveredSuccessful(const QSharedPointer<VersionIsReceived> &,
                                          const QH::AbstractNodeInfo *sender,
                                          const QH::Header &) {
    auto nodeInfo = dynamic_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
    nodeInfo->setFVersionDelivered(true);

    return true;
}

QH::AbstractNodeInfo *BaseNode::createNodeInfo(QAbstractSocket *socket,
                                               const QH::HostAddress *clientAddress) const {
    return new NodeInfo(socket, clientAddress);
}

void BaseNode::nodeConnected(QH::AbstractNodeInfo *node) {

    QH::AbstractNode::nodeConnected(node);

    ApplicationVersion appVersion;
    appVersion.setMaximum(maximumApiVersion());
    appVersion.setMinimum(minimumApiVersion());

    sendData(&appVersion, node);
}

int BaseNode::maximumApiVersion() const {
    if (_apiParsers.size()) {
        return _apiParsers.last()->version();
    }

    return 0;
}

int BaseNode::minimumApiVersion() const {
    if (_apiParsers.size()) {
        return _apiParsers.first()->version();
    }

    return 0;
}

QSharedPointer<QH::iParser>
BaseNode::selectParser(const ApplicationVersion &distVersion) const {
    for (int version = distVersion.maximum(); version >= distVersion.minimum(); --version) {
        auto parser = _apiParsers.value(version, nullptr);
        if (parser)
            return parser;
    }

    return nullptr;
}

void BaseNode::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                                QAbstractSocket::SocketError errorCode,
                                QString errorString) {

    QH::AbstractNode::nodeErrorOccured(nodeInfo, errorCode, errorString);

    if (QAbstractSocket::SocketError::SslInternalError == errorCode) {
        // see handleSslErrorOcurred
        return;
    }

    emit sigNetworkError(errorCode, QSslError::NoError);
}

void BaseNode::handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) {
    QH::AbstractNode::handleSslErrorOcurred(scket, error);
    emit sigNetworkError(QAbstractSocket::SocketError::SslInternalError,
                         error.error());
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

QSharedPointer<User> BaseNode::getUser(unsigned int userId) const {
    User request;
    request.setId(userId);

    return db()->getObject(request);
}

QList<QSharedPointer<UsersCards> > BaseNode::getAllUserData(unsigned int userId) const {
    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                    QString("user='%0'").arg(userId));

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return result->data();
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
BaseNode::getAllUserFromCard(unsigned int cardId) const {

    QString where = QString("card=%0").arg(cardId);

    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                  where);

    return _db->getObject(request)->data();
}

QList<QSharedPointer<User> >
BaseNode::getAllUserDataFromCard(unsigned int cardId) const {
    QString where = QString("card=%0").arg(cardId);

    where = QString("id IN (select user from UsersCards where %0)").arg(where);

    QH::PKG::DBObjectsRequest<User> request("Users",
                                            where);

    return _db->getObject(request)->data();
}

bool BaseNode::restoreOldData(const QByteArray &curentUserKey,
                              const QString &domain, int port) {

    auto action = [this, curentUserKey](QH::AbstractNodeInfo *node) {

        RestoreDataRequest request;
        request.setUserKey(curentUserKey);

        sendData(&request, node);
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

QSharedPointer<Card> BaseNode::getCard(unsigned int cardId) {
    Card request;
    request.setId(cardId);

    return _db->getObject(request);
}

QList<QSharedPointer<Card> > BaseNode::getAllUserCards(const QByteArray& userKey,
                                                       bool restOf) {

    QString where = "ownerSignature= '%0'";
    if (restOf) {
        where = "ownerSignature!= '%0'";
    }

    where = where.arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding)));

    QH::PKG::DBObjectsRequest<Card> cardRequest("Cards", where);
    if (auto result = db()->getObject(cardRequest)) {
        return result->data();
    }

    return {};
}

QList<QSharedPointer<UsersCards>> BaseNode::getAllUserCardsData(const QByteArray &userKey) {
    QString whereBlock = QString("card IN SELECT id FROM Cards WHERE ownerSignature = '%0'");
    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                  whereBlock.arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding))));
    auto result = db()->getObject(request);

    if (!result)
        return {};

    return result->data();
}

QByteArray BaseNode::getUserSecret(unsigned int userId) const {
    QH::PKG::GetSingleValue reqest({"Users", userId}, "secret");

    auto result = db()->getObject(reqest);
    return result->value().toByteArray();
}

void RC::BaseNode::addApiParser(const QSharedPointer<iParser>& api) {
    _apiParsers[api->version()] = api;
}

QString BaseNode::getServerHost() const {
    auto settings = QuasarAppUtils::ISettings::instance();

    if (!settings)
        return DEFAULT_CHEAT_CARD_HOST;

    if (!settings->getValue("devSettingEnable", false).toBool()) {
        return DEFAULT_CHEAT_CARD_HOST;
    }

    return settings->getStrValue("host", DEFAULT_CHEAT_CARD_HOST);
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
