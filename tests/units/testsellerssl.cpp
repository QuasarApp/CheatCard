#include "testsellerssl.h"
#include "testdatabasewrapper.h"

TestSellerSSL::TestSellerSSL(QSharedPointer<TestDataBaseWrapper> db):
    RC::SellerSSL(db->db()) {
    privateDb = db;

};
