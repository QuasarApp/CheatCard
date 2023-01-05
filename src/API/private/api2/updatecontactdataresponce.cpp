//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "updatecontactdataresponce.h"

namespace RC {
namespace API {
namespace V2 {

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
}
