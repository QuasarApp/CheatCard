#include "updatecontactdataresponce.h"

namespace RC {
namespace APIv1_5 {


UpdateContactDataResponce::UpdateContactDataResponce() {

}

bool UpdateContactDataResponce::isValid() const {
    return QH::PKG::AbstractData::isValid();
}

QDataStream &UpdateContactDataResponce::fromStream(QDataStream &stream) {

    stream >> successful;
    return stream;
}

QDataStream &UpdateContactDataResponce::toStream(QDataStream &stream) const {

    stream << successful;
    return stream;
}

bool UpdateContactDataResponce::getSuccessful() const {
    return successful;
}

void UpdateContactDataResponce::setSuccessful(bool newSuccessful) {
    successful = newSuccessful;
}
}
}
