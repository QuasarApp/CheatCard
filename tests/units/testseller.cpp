#include "testdatabasewrapper.h"
#include "testseller.h"

TestSeller::TestSeller(QSharedPointer<TestDataBaseWrapper> db): RC::Seller(db->db()) {
    privateDb = db;
}
