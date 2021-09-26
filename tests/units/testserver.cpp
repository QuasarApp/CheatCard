#include "testserver.h"

TestServer::TestServer(QH::ISqlDBCache * db): RC::Server(db) {

}

bool TestServer::isDataReceivedSuccessful() const {
    return _finished;
}
