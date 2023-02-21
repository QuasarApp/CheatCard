
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
#include <rci/objects/icontacts.h>
#include <rci/objects/iuser.h>

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

bool RCUtils::createContact(const QSharedPointer<Interfaces::iUser> &baseUser,
                            const QSharedPointer<Interfaces::iUser> &anotherUser,
                            QSharedPointer<Interfaces::iContacts> &resultContact) {
    if (!baseUser->isValid())
        return false;

    if (baseUser->getKey() == anotherUser->getKey()) {
        return false;
    }

    resultContact->setUserKey(baseUser->getKey());
    resultContact->setInfo(anotherUser->name());
    resultContact->setChildUserKey(anotherUser->getKey());

    return resultContact->isValid();
}

}
