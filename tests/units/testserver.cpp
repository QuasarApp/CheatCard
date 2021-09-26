#include "testdatabasewrapper.h"
#include "testserver.h"

#include <private/userscards.h>

TestServer::TestServer(QSharedPointer<TestDataBaseWrapper> db): RC::Server(db->db()) {
    privateDb = db;
}

bool TestServer::isDataReceivedSuccessful() const {
    return _finished;
}

int TestServer::getPurchaseCount(int userId, int cardId) {
    QSharedPointer<RC::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
