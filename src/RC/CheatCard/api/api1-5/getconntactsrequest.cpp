#include "getconntactsrequest.h"

namespace RC {
namespace APIv1_5 {
GetConntactsRequest::GetConntactsRequest() {

}

bool GetConntactsRequest::isValid() const {
    return userKey.size();
}

QDataStream &GetConntactsRequest::fromStream(QDataStream &stream) {
    stream >> userKey;

    return stream;
}

QDataStream &GetConntactsRequest::toStream(QDataStream &stream) const {
    stream << userKey;

    return stream;
}

const QByteArray &GetConntactsRequest::getUserKey() const {
    return userKey;
}

void GetConntactsRequest::setUserKey(const QByteArray &newUserKey) {
    userKey = newUserKey;
}

}
}
