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

#include <CheatCard/seller.h>


namespace RC {
namespace Interfaces {
class iUser;
}
}

void softDeleteWrapNode(RC::BaseNode* obj);

class CheatCardTestsHelper
{
public:
    CheatCardTestsHelper();

    template <class NodeType>
    static QSharedPointer<NodeType> makeNode(const QString database = "") {
        srand(time(0) + rand());
        QString randomNodeName = QByteArray::number(rand()).toHex() + typeid(NodeType).name();

        auto sallerDb = RC::DB::makeDb(1, randomNodeName, "", database);

        if (!sallerDb->init()) {
            return {};
        }

        return QSharedPointer<NodeType>(new NodeType(sallerDb), softDeleteWrapNode);
    }

    static QSharedPointer<RC::Interfaces::iUser> makeUser();
    static QSharedPointer<RC::Interfaces::iContacts> makeContact();

    static unsigned int testCardId();
    static unsigned int testUserId();
    static QByteArray testUserPublicKey();
    static QByteArray testUserPrivKey();

};

#endif // CHEATCARDTESTSHELPER_H
