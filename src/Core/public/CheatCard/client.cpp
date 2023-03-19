#include "client.h"
#include "api.h"
#include <params.h>
#include <settings.h>

namespace RC {

Client::Client(const QSharedPointer<Interfaces::iDB>& db): BaseNode(db) {
    auto apis = API::init({3}, db, this);

    for (const auto & api: apis) {
        Client::connect(api.data(), &API::APIBase::sigCardReceived,
                        this, &Client::sigCardReceived, Qt::DirectConnection);

        Client::connect(api.data(), &API::APIBase::sigUserDataRemoved,
                        this, &Client::sigUserDataRemoved, Qt::DirectConnection);

        Client::connect(api.data(), &API::APIBase::sigContactsListChanged,
                        this, &Client::sigContactsListChanged, Qt::DirectConnection);

        Client::connect(api.data(), &API::APIBase::sigPurchaseWasSuccessful,
                        this, &Client::sigPurchaseWasSuccessful, Qt::DirectConnection);

    }
}

Client::~Client() {
    if (_reconnetTimer) {
        _reconnetTimer->stop();
        delete _reconnetTimer;
        _reconnetTimer = nullptr;
    }
}

QString Client::getServerHost() const {
    auto settings = QuasarAppUtils::ISettings::instance();

    if (!settings)
        return DEFAULT_CHEAT_CARD_HOST;

    if (!settings->getValue("devSettingEnable", false).toBool()) {
        return DEFAULT_CHEAT_CARD_HOST;
    }

    return settings->getStrValue("host", DEFAULT_CHEAT_CARD_HOST);
}

bool Client::connectToServer(QString host, int port) {
    if (connectionsCount() >= 1) {
        return false;
    }

    auto action = [this](QH::AbstractNodeInfo * serverNode){
        _server = serverNode;

        if (_currntUserKey.size()) {
            subscribeToUser(_currntUserKey);
        }
    };

    if (host.isEmpty())
        host = getServerHost();

    if (!port) {
        port = DEFAULT_CHEAT_CARD_PORT;
    }

    return addNode(host, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

void Client::disconectFromServer() {
    removeNode(_server);
    if (_reconnetTimer) {
        _reconnetTimer->stop();
    }
}

bool Client::isConncted() const {
    return _server;
}

bool Client::subscribeToUser(const QByteArray& user) const {
    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    apiObject->syncRequest(user, _server, [](unsigned int err) {
        if (err) {
            QuasarAppUtils::Params::log("subscribe error ocurred");
        }
    });

    return true;
}

bool Client::updateCard(const QByteArray &cardId, unsigned int version) {

    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->sendUpdateCard(cardId, version, _server, [](unsigned int err) {
        if (err) {
            QuasarAppUtils::Params::log("updateCard error ocurred");
        }
    });
}

bool Client::requestCard(const QByteArray &cardId) {
    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->requestCard(cardId, _server, [](unsigned int err) {
        if (err) {
            QuasarAppUtils::Params::log("updateCard error ocurred");
        }
    });
}

bool Client::deleteCard(const QByteArray &cardId) {

    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->deleteCard(cardId, _currntUserKey, _server, [](unsigned int err) {
        if (err) {
            QuasarAppUtils::Params::log("deleteCard error ocurred");
        }
    });
}

QH::AbstractNode::NodeType Client::nodeType() const {
    return QH::AbstractNode::NodeType::Client;
}

bool Client::incrementPurchase(const QByteArray &userKey,
                               const QByteArray &cardId,
                               int purchasesCount,
                               const std::function<void (int)> &cb) {

    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->changeUsersData(_currntUserKey,
                                      cardId,
                                      userKey,
                                      purchasesCount,
                                      0,
                                      _server,
                                      [cb](unsigned int err) {
                                          if (err) {
                                              QuasarAppUtils::Params::log("deleteCard error ocurred");
                                          }

                                          cb(err);
                                      });
}

bool Client::incrementReceived(const QByteArray &userKey,
                               const QByteArray &cardId,
                               int received,
                               const std::function<void(int err)>& cb) {
    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->changeUsersData(_currntUserKey,
                                      cardId,
                                      userKey,
                                      0,
                                      received,
                                      _server,
                                      [cb](unsigned int err) {
                                          if (err) {
                                              QuasarAppUtils::Params::log("deleteCard error ocurred");
                                          }
                                          cb(err);
                                      });
}

QSharedPointer<Interfaces::iAPI> Client::api() const {
    return selectParser(API_BASE_PARSE_IS, _server).dynamicCast<Interfaces::iAPI>();
}

QH::AbstractNodeInfo *Client::server() const {
    return _server;
}

void Client::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}

void Client::nodeConfirmend(QH::AbstractNodeInfo *node){
    BaseNode::nodeConfirmend(node);
    setFNetAvailable(confirmendCount());
}

void Client::nodeDisconnected(QH::AbstractNodeInfo * node) {
    BaseNode::nodeDisconnected(node);
    setFNetAvailable(confirmendCount());
    _server = nullptr;

}

void Client::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                              QAbstractSocket::SocketError errorCode,
                              QString errorString) {
    BaseNode::nodeErrorOccured(nodeInfo, errorCode, errorString);

    if (!_reconnetTimer) {
        _reconnetTimer = new QTimer();
    }

    _reconnetTimer->singleShot(2000, this, [this](){
        connectToServer();
    });
}

void Client::setFNetAvailable(bool newFNetAvailable) {
    if (_fNetAvailable == newFNetAvailable) {
        return;
    }

    _fNetAvailable = newFNetAvailable;

    emit sigAvailableNetworkChanged(_fNetAvailable);
}

const QByteArray &Client::currntUserKey() const {
    return _currntUserKey;
}

void Client::setCurrntUserKey(const QByteArray &newCurrntUserKey) {
    _currntUserKey = newCurrntUserKey;

    if (_currntUserKey.size()) {
        subscribeToUser(_currntUserKey);
    }
}

bool RC::Client::updateContactData(const QSharedPointer<Interfaces::iContacts> &contact,
                                   const QByteArray &secreet,
                                   bool removeRequest,
                                   const std::function<void(int err)>& cb) {

    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->sendContacts(contact,
                                   secreet,
                                   removeRequest,
                                   _server,
                                   [cb](unsigned int err) {
                                       if (err) {
                                           QuasarAppUtils::Params::log("updateContactData error ocurred");
                                       }
                                       cb(err);

                                   });
}

bool Client::grantAccess(const QSharedPointer<Interfaces::iUser> &parent,
                         const QByteArray &child,
                         const QString& name,
                         const std::function<void(int err)>& cb) {
    if (!db())
        return false;

    auto contact = db()->makeEmptyContact();

    contact->setChildUserKey(child);
    contact->setUserKey(parent->getKey());
    contact->setInfo(name);

    return updateContactData(contact, parent->secret(), false, cb);
}

bool Client::dropAccess(const QSharedPointer<Interfaces::iUser> &parent,
                        const QByteArray &child,
                        const std::function<void(int err)>& cb) {
    if (!db())
        return false;

    auto contact = db()->makeEmptyContact();

    contact->setChildUserKey(child);
    contact->setUserKey(parent->getKey());

    return updateContactData(contact, parent->secret(), true, cb);
}

}
