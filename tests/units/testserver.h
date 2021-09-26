#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/server.h>

class TestServer: public RC::Server
{
    Q_OBJECT
public:
    TestServer(QH::ISqlDBCache*);
    bool isDataReceivedSuccessful() const;
private:
    bool _finished = false;
};

#endif // TESTSERVER_H
