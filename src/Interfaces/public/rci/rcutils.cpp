
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
unsigned int RCUtils::makeOlduserId(const QByteArray &keys) {
    return qHash(keys);
}
#endif

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

    return createContact(baseUser->getKey(), anotherUser->getKey(), anotherUser->getKey(), resultContact);
}

bool RCUtils::createContact(const QByteArray &baseUser,
                            const QByteArray &anotherUser,
                            const QString &info,
                            QSharedPointer<Interfaces::iContacts> &resultContact) {

    if (baseUser == anotherUser) {
        return false;
    }

    resultContact->setUserKey(baseUser);
    resultContact->setInfo(info);
    resultContact->setChildUserKey(anotherUser);

    return resultContact->isValid();
}

unsigned int RCUtils::calcFreeItemsCount(unsigned int received, unsigned int purchasesNumber, unsigned int freeIndex) {
    if (freeIndex <= 0)
        return 0;

    if (!purchasesNumber)
        return 0;

    int freeItems = std::floor(purchasesNumber /
                               static_cast<double>(freeIndex)) -
                    received;

    return freeItems;
}

}
