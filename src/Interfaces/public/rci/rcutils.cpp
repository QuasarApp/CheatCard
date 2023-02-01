
//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "rcutils.h"
#include <crc/crchash.h>
#include <QCryptographicHash>
#include <qaglobalutils.h>

namespace RC {

RCUtils::RCUtils() {

}

QByteArray RCUtils::makeUserKey(const QByteArray &secret) {
    return QCryptographicHash::hash(secret, QCryptographicHash::Sha256);
}

QByteArray RCUtils::randomSHA256() {
    QByteArray result;
    randomArray(32, result);
    return result;
}

QByteArray RCUtils::convrtOldIdToSHA256(unsigned int oldId) {
    return QCryptographicHash::hash(QByteArray::number(oldId, sizeof(oldId)),
                                    QCryptographicHash::Sha256);
}

}
