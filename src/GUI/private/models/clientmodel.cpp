#include "clientmodel.h"

namespace RC {

ClientModel::ClientModel(const QSharedPointer<Interfaces::iDB>& db):
    Client(db, {3}) {

}
}
