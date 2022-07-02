#include "getconntactsrequest.h"

namespace RC {
namespace APIv1_5 {
GetConntactsRequest::GetConntactsRequest() {

}

bool GetConntactsRequest::isValid() const {
    return userId;
}

QDataStream &GetConntactsRequest::fromStream(QDataStream &stream) {
    stream >> userId;

    return stream;
}

QDataStream &GetConntactsRequest::toStream(QDataStream &stream) const {
    stream << userId;

    return stream;
}

unsigned int GetConntactsRequest::getUserId() const {
    return userId;
}

void GetConntactsRequest::setUserId(unsigned int newUserId) {
    userId = newUserId;
}

}
}
