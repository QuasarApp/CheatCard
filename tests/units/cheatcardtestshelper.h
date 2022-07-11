#ifndef CHEATCARDTESTSHELPER_H
#define CHEATCARDTESTSHELPER_H

#include "testdatabasewrapper.h"

#include <QSharedPointer>

#include <CheatCard/seller.h>


namespace RC {
namespace API {
class User;
}
}

void softDeleteWrapDB(TestDataBaseWrapper* obj);
void softDeleteWrapNode(RC::BaseNode* obj);

class CheatCardTestsHelper
{
public:
    CheatCardTestsHelper();

    template <class NodeType>
    static QSharedPointer<NodeType> makeNode(const QString database = ":/sql/units/sql/TestSallerDb.sql",
                                             bool forceUseCustomDb = false) {
        srand(time(0) + rand());
        QString randomNodeName = QByteArray::number(rand()).toHex() + typeid(NodeType).name();

        TestDataBaseWrapper* source;
        if (std::is_base_of<RC::Seller,NodeType>::value || forceUseCustomDb) {

            source = new TestDataBaseWrapper(randomNodeName,
                                             database);
        } else {
            source = new TestDataBaseWrapper(randomNodeName);
        }

        auto sallerDb = QSharedPointer<TestDataBaseWrapper>(source,
                                                            softDeleteWrapDB);
        if (!sallerDb->initSqlDb()) {
            return {};
        }

        return QSharedPointer<NodeType>(new NodeType(sallerDb), softDeleteWrapNode);
    }

    static QSharedPointer<RC::API::User> makeUser();
    static unsigned int testCardId();
    static unsigned int testUserId();
    static QByteArray testUserPublicKey();
    static QByteArray testUserPrivKey();

};

#endif // CHEATCARDTESTSHELPER_H
