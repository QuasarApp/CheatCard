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
#include <CheatCard/api/api0/session.h>

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscards.h>
#include <CheatCard/api/api1/card.h>

#include "CheatCard/nodeinfo.h"
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

#include <CheatCard/api/apiv1-5.h>
#include <CheatCard/api/apiv1.h>

#include <CheatCard/api/api1-5/updatecontactdata.h>
#include <QCryptographicHash>

namespace RC {

BaseNode::BaseNode(QH::ISqlDBCache *db): APIObjectsFactoryV1(db) {
    _db = db;

    registerPackageType<ApplicationVersion>();
    registerPackageType<VersionIsReceived>();


    registerPackageType<QH::PKG::DataPack<API::UsersCards>>();
    registerPackageType<QH::PKG::DataPack<APIv1::UsersCards>>();

    registerPackageType<QH::PKG::DataPack<API::Card>>();
    registerPackageType<QH::PKG::DataPack<APIv1::Card>>();

    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                       << QSslError::SelfSignedCertificateInChain
                       << QSslError::HostNameMismatch
                       << QSslError::CertificateUntrusted);
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
    auto parser = selectParser(distVersion);

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

    auto distVersion = *message.data();

    nodeInfo->setVersion(distVersion);
    auto parser = selectParser(distVersion);

    if (!parser) {
        QuasarAppUtils::Params::log(QString("Can't found requeried parser for versions: %0-%1").
                                    arg(distVersion.minimum()).
                                    arg(distVersion.maximum()),
                                    QuasarAppUtils::Error);

        int localMaximumVersion = 0;
        if (_apiParsers.size()) {
            localMaximumVersion = _apiParsers.lastKey();
        }

        if (nodeInfo->version().minimum() > localMaximumVersion) {
            emit sigVersionNoLongerSupport(nodeInfo->version().minimum());
        }

        removeNode(sender->networkAddress());

        return false;
    }

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

    emit sigDataExchangingChanged(connectionsCount());
}

void BaseNode::nodeDisconnected(QH::AbstractNodeInfo * node) {
    QH::AbstractNode::nodeDisconnected(node);

    emit sigDataExchangingChanged(connectionsCount());
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

void BaseNode::addNodeFailed(QH::AddNodeError) {
    setFNetAvailable(false);
}

void BaseNode::nodeAddedSucessful(QH::AbstractNodeInfo *) {
    setFNetAvailable(true);
}

void BaseNode::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                                QAbstractSocket::SocketError errorCode,
                                QString errorString) {

    QH::AbstractNode::nodeErrorOccured(nodeInfo, errorCode, errorString);

    if (QAbstractSocket::SocketError::SslInternalError == errorCode) {
        // see handleSslErrorOcurred
        return;
    }

    setFNetAvailable(false);

    emit sigNetworkError(errorCode, QSslError::NoError);
}

void BaseNode::handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) {
    QH::AbstractNode::handleSslErrorOcurred(scket, error);
    emit sigNetworkError(QAbstractSocket::SocketError::SslInternalError,
                         error.error());
}

bool BaseNode::fNetAvailable() const {
    return _fNetAvailable;
}

void BaseNode::checkNetworkConnection(const QString &domain, int port) {
    auto action = [this](QH::AbstractNodeInfo *node) {
        removeNode(node);
    };

    if (domain.isEmpty()) {
        addNode(getServerHost(), port, action,
                QH::NodeCoonectionStatus::Confirmed);
        return;
    }

    addNode(domain, port, action,
            QH::NodeCoonectionStatus::Confirmed);
}

void BaseNode::setFNetAvailable(bool newFNetAvailable) {
    if (_fNetAvailable == newFNetAvailable) {
        return;
    }

    _fNetAvailable = newFNetAvailable;

    emit sigAvailableNetworkChanged(_fNetAvailable);
}

const QSharedPointer<API::User>& BaseNode::currentUser() const {
    return _currentUser;
}

void BaseNode::setCurrentUser(QSharedPointer<API::User> newCurrentUser) {
    _currentUser = newCurrentUser;
}

int BaseNode::getFreeItemsCount(unsigned int userId,
                                unsigned int cardId) const {
    return getFreeItemsCount(getUserCardData(userId, cardId));
}

int BaseNode::getFreeItemsCount(const QSharedPointer<API::UsersCards> &inputData) const {
    if (!inputData)
        return 0;

    unsigned int freeIndex = getCardFreeIndex(inputData->getCard());
    return getFreeItemsCount(inputData, freeIndex);
}

int BaseNode::getFreeItemsCount(const QSharedPointer<API::UsersCards> &inputData,
                                unsigned int freeIndex) const {
    if (freeIndex <= 0)
        return 0;

    if (!inputData)
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

void BaseNode::removeCard(const QSharedPointer<API::Card> &objUserCard) {
    _db->deleteObject(objUserCard);
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

bool BaseNode::restoreAllData(const QByteArray &curentUserKey,
                              const QString &domain, int port) {

    auto action = [this, curentUserKey](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = selectParser(dist->version()).dynamicCast<ApiV1>();
        if (api) {
            api->restoreOldDateRequest(curentUserKey, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool BaseNode::getContactsList(const QByteArray& userKey,
                               const QString &domain, int port) {

    auto action = [this, userKey](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = selectParser(dist->version()).dynamicCast<ApiV1_5>();
        if (api) {
            api->requestContacts(userKey, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool BaseNode::updateContactData(const API::Contacts &contact,
                                 const QByteArray& secreet,
                                 bool removeRequest,
                                 const QString &domain, int port) {

    auto action = [this, contact, secreet, removeRequest](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = selectParser(dist->version()).dynamicCast<ApiV1_5>();
        if (api) {
            api->sendContacts(contact, secreet, removeRequest, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool BaseNode::restoreOneCard(unsigned int cardId, const QString &domain, int port) {
    auto action = [this, cardId](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = selectParser(dist->version()).dynamicCast<ApiV1>();
        if (api) {
            api->restoreOneCardRequest(cardId, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

QByteArray BaseNode::getUserSecret(unsigned int userId) const {
    QH::PKG::GetSingleValue reqest({"Users", userId}, "secret");

    auto result = db()->getObject(reqest);
    return result->value().toByteArray();
}

QSharedPointer<QH::iParser> BaseNode::getSelectedApiParser(QH::AbstractNodeInfo *node) const {
    auto distVersion = static_cast<const NodeInfo*>(node)->version();
    return selectParser(distVersion);
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

void BaseNode::init() {
    initCheatCardBaseResources();
}

bool BaseNode::createChilduser(const QString &description,
                                QSharedPointer<API::User> &resultChilduserAccount,
                                QSharedPointer<API::Contacts> &resultContact) {

    if (!_currentUser->isValid())
        return false;

    // generate new genesis number
    int genesis = rand();

    auto inputData = _currentUser->secret();
    inputData.insert(inputData.size(), reinterpret_cast<char*>(&genesis), sizeof (genesis));

    resultChilduserAccount->setSecret(QCryptographicHash::hash(inputData,
                                                               QCryptographicHash::Sha256));
    resultChilduserAccount->regenerateKeys();
    resultChilduserAccount->setName(description);

    if (!resultChilduserAccount->isValid())
        return false;

    resultContact->setUserKey(_currentUser->getKey());
    resultContact->setGenesisKey(genesis);
    resultContact->setInfo(description);
    resultContact->setChildUserKey(resultChilduserAccount->getKey());

    return resultContact->isValid();
}

const QMap<int, QSharedPointer<QH::iParser> > &BaseNode::apiParsers() const {
    return _apiParsers;
}

void BaseNode::setApiParsers(const QMap<int, QSharedPointer<QH::iParser> > &newApiParsers) {
    _apiParsers = newApiParsers;
}


}
