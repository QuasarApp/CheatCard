//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CHEATCARDTESTSHELPER_H
#define CHEATCARDTESTSHELPER_H

#include <db.h>
#include <QSharedPointer>

#include <CheatCard/client.h>

class TestClient;
class TestServer;

namespace RC {
namespace Interfaces {
class iUser;
}
}

void softDeleteWrapNode(RC::BaseNode* obj);

struct NetworkResult {
    QSharedPointer<TestServer> server;
    QHash<QByteArray, QSharedPointer<TestClient>> clients;
};

class CheatCardTestsHelper
{
public:
    CheatCardTestsHelper();

    template <class NodeType>
    static QSharedPointer<NodeType> makeNode(const QString database = "") {
        srand(time(0) + rand());
        QString randomNodeName = QByteArray::number(rand()).toHex() + typeid(NodeType).name();

        auto sallerDb = RC::DB::makeDb(1, randomNodeName, "", database);
        auto user = makeUser();


        if (!sallerDb->init()) {
            return {};
        }

        if (!sallerDb->saveUser(user)) {
            return false;
        }

        auto result = QSharedPointer<NodeType>(new NodeType(sallerDb), softDeleteWrapNode);
        result->setCurrntUserKey(user->getKey());
        return result;
    }

    static NetworkResult deployNetwork(QString host, int port, unsigned int clientsCount = 2);

    static QSharedPointer<RC::Interfaces::iUser> makeUser();
    static QSharedPointer<RC::Interfaces::iContacts> makeContact();
    static QSharedPointer<RC::Interfaces::iCard> makeCard(const QSharedPointer<TestClient> &owner, unsigned int freeItemCount);
    static void makeSeals(const QSharedPointer<TestClient>& seller,
                          const QSharedPointer<TestClient>& client,
                          const QByteArray& card,
                          unsigned int sealCount);

private:
    static const QSharedPointer<RC::Interfaces::iDB> &objectFactory();


};

#endif // CHEATCARDTESTSHELPER_H
