#include "appversion.h"
namespace RC {

QDataStream &AppVersion::fromStream(QDataStream &stream) {
    stream >> _version;

    return stream;
}

QDataStream &AppVersion::toStream(QDataStream &stream) const {
    stream << _version;

    return stream;
}

unsigned short AppVersion::version() const {
    return _version;
}

void AppVersion::setVersion(unsigned short newVersion) {
    _version = newVersion;
}
}
