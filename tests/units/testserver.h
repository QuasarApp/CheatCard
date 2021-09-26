#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/server.h>

class TestDataBaseWrapper;

class TestServer: public RC::Server
{
    Q_OBJECT
public:
    TestServer(QSharedPointer<TestDataBaseWrapper>);
    bool isDataReceivedSuccessful() const;
    int getPurchaseCount(int userId, int cardId);

private:
    bool _finished = false;
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSERVER_H
