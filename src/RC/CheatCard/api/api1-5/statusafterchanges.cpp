#include "statusafterchanges.h"


namespace RC {
namespace APIv1_5 {

StatusAfterChanges::StatusAfterChanges() {

}

RC::APIv1_5::StatusAfterChanges::~StatusAfterChanges() {

}

bool StatusAfterChanges::status() const {
    return _status;
}

void StatusAfterChanges::setStatus(bool newStatus) {
    _status = newStatus;
}

QDataStream &StatusAfterChanges::fromStream(QDataStream &stream) {
    API::Session::fromStream(stream);
    stream >> _status;
    return stream;
}

QDataStream &StatusAfterChanges::toStream(QDataStream &stream) const {
    API::Session::toStream(stream);
    stream << _status;
    return stream;
}

}
}
