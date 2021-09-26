#include "testvisitor.h"

#include <testdatabasewrapper.h>

TestVisitor::TestVisitor(QSharedPointer<TestDataBaseWrapper> db): RC::Visitor(db->db()) {
        privateDb = db;
}
