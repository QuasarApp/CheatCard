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

QString Client::getServerHost() const {
    auto settings = QuasarAppUtils::ISettings::instance();

    if (!settings)
        return DEFAULT_CHEAT_CARD_HOST;

    if (!settings->getValue("devSettingEnable", false).toBool()) {
        return DEFAULT_CHEAT_CARD_HOST;
    }

    return settings->getStrValue("host", DEFAULT_CHEAT_CARD_HOST);
}

bool Client::connectToServer() {
    if (connectionsCount() >= 1) {
        return false;
    }

    auto action = [this](QH::AbstractNodeInfo * serverNode){
        _server = serverNode;

        if (_currntUserKey.size()) {
            subscribeToUser(_currntUserKey);
        }
    };

    return addNode(getServerHost(), DEFAULT_CHEAT_CARD_PORT, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

void Client::disconectFromServer() {
    removeNode(_server);
}

bool Client::isConncted() const {
    return confirmendCount();
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

bool Client::setPurchase(const UserHeader &userHeaderData,
                         const QByteArray &cardId,
                         int purchasesCount) {

    auto apiObject = api();
    if (!apiObject) {
        return false;
    }

    return apiObject->(cardId, _currntUserKey, _server, [](unsigned int err) {
        if (err) {
            QuasarAppUtils::Params::log("deleteCard error ocurred");
        }
    });
}

bool Client::incrementPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                               unsigned int cardId,
                               int purchasesCount) {

}

QSharedPointer<Interfaces::iAPI> Client::api() const {
    return selectParser(API_BASE_PARSE_IS, _server).dynamicCast<Interfaces::iAPI>();
}

QH::AbstractNodeInfo *Client::server() const {
    return _server;
}

void Client::nodeConnected(QH::AbstractNodeInfo *node) {

    BaseNode::nodeConnected(node);
    setFNetAvailable(confirmendCount());
}

void Client::nodeDisconnected(QH::AbstractNodeInfo * node) {
    BaseNode::nodeDisconnected(node);
    setFNetAvailable(confirmendCount());
    _server = nullptr;

}

void Client::setFNetAvailable(bool newFNetAvailable) {
    if (_fNetAvailable == newFNetAvailable) {
        return;
    }

    _fNetAvailable = newFNetAvailable;

    emit sigAvailableNetworkChanged(_fNetAvailable);
}

void Client::setCurrntUserKey(const QByteArray &newCurrntUserKey) {
    _currntUserKey = newCurrntUserKey;

    if (_currntUserKey.size()) {
        subscribeToUser(_currntUserKey);
    }
}

}
