#include "client.h"
#include "api.h"
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
    };

    return addNode(getServerHost(), DEFAULT_CHEAT_CARD_PORT, action);
}

void Client::disconectFromServer() {
    removeNode(_server);
}

bool Client::isConncted() const {
    return connectionsCount();
}

QSharedPointer<Interfaces::iAPI> Client::api() const {
    return selectParser(API_BASE_PARSE_IS, _server).dynamicCast<Interfaces::iAPI>();
}

QH::AbstractNodeInfo *Client::server() const {
    return _server;
}

void Client::nodeConnected(QH::AbstractNodeInfo *node) {

    BaseNode::nodeConnected(node);
    setFNetAvailable(connectionsCount());
}

void Client::nodeDisconnected(QH::AbstractNodeInfo * node) {
    BaseNode::nodeDisconnected(node);
    setFNetAvailable(connectionsCount());
}

void Client::setFNetAvailable(bool newFNetAvailable) {
    if (_fNetAvailable == newFNetAvailable) {
        return;
    }

    _fNetAvailable = newFNetAvailable;

    emit sigAvailableNetworkChanged(_fNetAvailable);
}


}
