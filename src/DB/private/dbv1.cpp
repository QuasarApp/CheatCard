
//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "dbv1.h"
#include "objects/card.h"
#include "objects/contacts.h"
#include "objects/user.h"
#include <cmath>
#include <dbobjectsrequest.h>
#include <getsinglevalue.h>
#include <objects/userscards.h>
#include <pills/invalidcardidpill.h>
#include <pills/invaliduserspill.h>
#include <rci/rcutils.h>


namespace RC {

template<class InType, class DataType>
bool saveObject(const QSharedPointer<InType>& obj, QH::ISqlDB * db) {

    if (!db)
        return false;

    if (auto castedObj = obj.template dynamicCast<DataType>()) {
        return db->replaceObject(castedObj, true);
    }

    return db->replaceObject(QSharedPointer<DataType>::create(obj), true);
}

DBv1::DBv1(const QString &name,
           const QString &backUpLocation,
           const QString &custoSql):
    DB::AppDataBase(name, backUpLocation, custoSql) {}

bool DBv1::init() {
    return initSqlDb();
}

bool DBv1::isInit() const {
    return isSqlInited();
}

bool DBv1::deleteContact(const QSharedPointer<Interfaces::iContacts> &contact) const {
    if(!db())
        return false;

    return db()->deleteObject(QSharedPointer<DB::Contacts>::create(contact));
}

bool DBv1::deleteCard(const QByteArray &cardId) const {
    if(!db())
        return false;

    QString base64Id = cardId.toBase64(QByteArray::Base64UrlEncoding);
    if (!db()->doQuery(QString("DELETE FROM Cards WHERE id ='%0'").
                       arg(base64Id))) {
        return false;
    }

    if (!db()->doQuery(QString("DELETE FROM UsersCards WHERE card ='%0'").
                       arg(base64Id))) {
        return false;
    }

    return true;
}

bool DBv1::deleteContactsByChildUserKey(const QByteArray &childUser) const {
    if(!db())
        return false;

    return db()->doQuery(QString("DELETE FROM Contacts WHERE childUserKey= '%0'").
                         arg(QString(childUser.toBase64(QByteArray::Base64UrlEncoding))));
}

bool DBv1::deleteUserData(const QByteArray &cardId, const QByteArray &userId) {
    DB::UsersCards(userId, cardId);

    if (!db())
        return false;

    return db()->deleteObject(QSharedPointer<DB::UsersCards>::create(userId, cardId));
}

QSharedPointer<Interfaces::iContacts> DBv1::makeEmptyContact() const {
    return QSharedPointer<DB::Contacts>::create();
}

QSharedPointer<Interfaces::iUser> DBv1::makeEmptyUser() const {
    return QSharedPointer<DB::User>::create();
}

QSharedPointer<Interfaces::iCard> DBv1::makeEmptyCard() const {
    return QSharedPointer<DB::Card>::create();
}

QSharedPointer<Interfaces::iUsersCards> DBv1::makeEmptyUsersCard() const {
    return QSharedPointer<DB::UsersCards>::create();
}

bool DBv1::saveUser(const QSharedPointer<Interfaces::iUser> &user) const {
    return saveObject<Interfaces::iUser, DB::User>(user, db());
}

bool DBv1::saveCard(const QSharedPointer<Interfaces::iCard>& card) const {
    return saveObject<Interfaces::iCard, DB::Card>(card, db());
}

bool DBv1::saveUsersCard(const QSharedPointer<Interfaces::iUsersCards>& userData) const {
    return saveObject<Interfaces::iUsersCards, DB::UsersCards>(userData, db());
}

bool DBv1::saveContact(const QSharedPointer<Interfaces::iContacts> &contact) const {
    return saveObject<Interfaces::iContacts, DB::Contacts>(contact, db());

}

int DBv1::getFreeItemsCount(const QSharedPointer<Interfaces::iUsersCards> &inputData) const {
    if (!inputData)
        return 0;

    unsigned int freeIndex = getCardFreeIndex(inputData->getCard());
    return getFreeItemsCount(inputData, freeIndex);
}

int DBv1::getFreeItemsCount(const DB::UsersCards &inputData, unsigned int freeIndex) const {
    if (freeIndex <= 0)
        return 0;

    int freeItems = std::floor(inputData.getPurchasesNumber() /
                               static_cast<double>(freeIndex)) -
            inputData.getReceived();

    return freeItems;
}

int DBv1::getCountOfReceivedItems(const QByteArray &userId, const QByteArray &cardId) {
    return getUserCardData(userId, cardId)->getReceived();
}

int DBv1::getCardFreeIndex(const QByteArray &cardId) const {
    QH::PKG::GetSingleValue request({"Cards", QString(cardId.toBase64(QByteArray::Base64UrlEncoding))}, "freeIndex");
    auto result = db()->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

unsigned int DBv1::getCardVersion(const QByteArray &cardId) const {
    QH::PKG::GetSingleValue request({"Cards", cardId}, "cardVersion");
    auto result = db()->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

QVariant DBv1::getCardField(const QByteArray &cardId, const QString &field) {
    QH::PKG::GetSingleValue request(QH::DbAddress("cards", cardId), field);
    auto dbObj = db()->getObject(request);

    if (!dbObj)
        return {};

    return dbObj->value();
}

QList<QSharedPointer<Interfaces::iUsersCards> >
DBv1::getAllUserData(const QByteArray &userId) const {
    QH::PKG::DBObjectsRequest<DB::UsersCards>
            request("UsersCards",
                    QString("user='%0'").
                    arg(QString(userId.toBase64(QByteArray::Base64UrlEncoding))));

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};
}

QSharedPointer<Interfaces::iUsersCards>
DBv1::getUserCardData(const QByteArray& userId, const QByteArray& cardId) const {

    DB::UsersCards request;
    request.setCard(cardId);
    request.setUser(userId);

    auto purches = db()->getObject(request);
    return purches;

}

QList<QSharedPointer<Interfaces::iUsersCards> >
DBv1::getAllUserFromCard(const QByteArray &cardId, const QByteArray &ignoreUserId) const {
    QString where;
    QString base64CardId = cardId.toBase64(QByteArray::Base64UrlEncoding);
    if (ignoreUserId.size()) {
        where = QString("card=%0 AND user!=%1").
                arg(base64CardId,
                    ignoreUserId.toBase64(QByteArray::Base64UrlEncoding));
    } else {
        where = QString("card=%0").arg(base64CardId);
    }

    QH::PKG::DBObjectsRequest<DB::UsersCards> request("UsersCards",
                                                      where);

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};
}

QList<QSharedPointer<Interfaces::iUsersCards> >
DBv1::getAllPassiveUserFromCard(const QByteArray &cardId,
                                int unixTimeRange,
                                const QByteArray &ignoreUserId) const {
    int timePoint = time(0) - unixTimeRange;
    QString base64CardId = cardId.toBase64(QByteArray::Base64UrlEncoding);

    QString where;
    if (ignoreUserId.size()) {
        where = QString("card=%0 AND time<%1 AND user!=%2").
                arg(base64CardId).
                arg(timePoint).
                arg(QString(ignoreUserId.toBase64(QByteArray::Base64UrlEncoding)));
    } else {
        where = QString("card=%0 AND time<%1")
                .arg(base64CardId).arg(timePoint);
    }

    QH::PKG::DBObjectsRequest<DB::UsersCards> request("UsersCards",
                                                      where);

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};
}

QList<QSharedPointer<Interfaces::iUsersCards> >
DBv1::getAllActiveUserFromCard(const QByteArray &cardId,
                               int unixTimeRange,
                               const QByteArray &ignoreUserId) const {
    int timePoint = time(0) - unixTimeRange;
    QString base64CardId = cardId.toBase64(QByteArray::Base64UrlEncoding);

    QString where;
    if (ignoreUserId.size()) {
        where = QString("card=%0 AND time>%1 AND user!=%2").
                arg(base64CardId).
                arg(timePoint).
                arg(QString(ignoreUserId.toBase64(QByteArray::Base64UrlEncoding)));
    } else {
        where = QString("card=%0 AND time>%1")
                .arg(base64CardId).arg(timePoint);
    }

    QH::PKG::DBObjectsRequest<DB::UsersCards> request("UsersCards",
                                                      where);

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};
}

QList<QSharedPointer<Interfaces::iUsersCards> >
DBv1::getAllUserCardsData(const QByteArray &userKey,
                          const QList<QSharedPointer<Interfaces::iContacts>> &childs) {

    QList<QByteArray> keys;

    for (const auto& key: childs) {
        keys.push_back(key->getUserKey());
    }
    keys.push_back(userKey);

    QString whereBlock = QString("card IN (SELECT id FROM Cards WHERE %0)");
    QString where;
    for (const QByteArray& key: keys) {
        if (where.isEmpty()) {
            where += QString{"ownerSignature= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
        } else {
            where += QString{" OR ownerSignature= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
        }
    }

    QH::PKG::DBObjectsRequest<DB::UsersCards>
            request("UsersCards", whereBlock.arg(where));

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};

}

QSharedPointer<Interfaces::iCard>
DBv1::getCard(const QByteArray& cardId) {
    DB::Card request;
    request.setCardId(cardId);

    return db()->getObject(request);

}

QList<QSharedPointer<Interfaces::iCard> >
DBv1::getAllUserCards(const QByteArray &userKey, bool restOf,
                      const QList<QSharedPointer<Interfaces::iContacts> > &childs) {

    QList<QByteArray> keys;
    for (const auto& key: childs) {
        keys.push_back(key->getUserKey());
    }
    keys.push_back(userKey);

    QString where;
    if (restOf) {

        for (const QByteArray& key: keys) {
            if (where.isEmpty()) {
                where += QString{"ownerSignature!= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
            } else {
                where += QString{" AND ownerSignature!= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
            }
        }

    } else {
        for (const QByteArray& key: keys) {
            if (where.isEmpty()) {
                where += QString{"ownerSignature= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
            } else {
                where += QString{" OR ownerSignature= '%0'"}.arg(QString(key.toBase64(QByteArray::Base64UrlEncoding)));
            }
        }
    }

    QH::PKG::DBObjectsRequest<DB::Card> cardRequest("Cards", where);

    auto result = db()->getObject(cardRequest);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};

}

QSharedPointer<Interfaces::iUser> DBv1::getUser(const QByteArray& userId) const {
    DB::User request;
    request.setKey(userId);

    return db()->getObject(request);
}

QList<QSharedPointer<Interfaces::iUser> >
DBv1::getAllUserDataFromCard(const QByteArray& cardId) const {
    QString where = QString("card=%0").arg(QString(cardId.toBase64(QByteArray::Base64UrlEncoding)));

    where = QString("id IN (select user from UsersCards where %0)").arg(where);

    QH::PKG::DBObjectsRequest<DB::User> request("Users",
                                                where);

    auto result = db()->getObject(request);

    if (!result)
        return {};

    return {result->data().begin(), result->data().end()};
}

QList<QSharedPointer<Interfaces::iUser> >
DBv1::getAllUserWithPrivateKeys() const {
    QH::PKG::DBObjectsRequest<DB::User> request("Users",
                                                "secret IS NOT NULL AND secret != \"\"");
    auto result = db()->getObject(request);
    if (!result)
        return {};

    return {result->data().begin(), result->data().end()} ;
}

QSharedPointer<Interfaces::iContacts>
DBv1::getContactFromChildId(const QByteArray& userKey, const QByteArray& childUserKey) {

    QString whereBlock = QString("userKey='%0' AND childUserKey='%1'").
            arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding)),
                QString(childUserKey.toBase64(QByteArray::Base64UrlEncoding)));

    QH::PKG::DBObjectsRequest<DB::Contacts>
            request("Contacts", whereBlock);

    auto result = db()->getObject(request);

    if (!result || result->data().isEmpty())
        return {};

    return *result->data().begin();
}

QList<QSharedPointer<Interfaces::iContacts> >
DBv1::getMasterKeys(const QByteArray& childUserKey) {
    QString whereBlock = QString("childUserKey='%0'").
            arg(QString(childUserKey.toBase64(QByteArray::Base64UrlEncoding)));

    QH::PKG::DBObjectsRequest<DB::Contacts>
            request("Contacts", whereBlock);

    auto result = db()->getObject(request);

    if (!result || result->data().isEmpty())
        return {};

    return {result->data().begin(), result->data().end()};
}

QList<QSharedPointer<Interfaces::iContacts> > DBv1::getSlaveKeys(const QByteArray &userKey) {
    QString whereBlock = QString("userKey='%0'").
            arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding)));

    QH::PKG::DBObjectsRequest<DB::Contacts>
            request("Contacts", whereBlock);

    auto result = db()->getObject(request);

    if (!result || result->data().isEmpty())
        return {};

    return {result->data().begin(), result->data().end()};
}

QByteArray DBv1::getSecretOfCardOvner(const QByteArray& cardId) const {
    QString whereBlock = QString("key IN (SELECT ownerSignature FROM Cards WHERE id=%0)").
            arg(QString(cardId.toBase64(QByteArray::Base64UrlEncoding)));

    QH::PKG::DBObjectsRequest<DB::User>
            request("Users", whereBlock);

    auto result = db()->getObject(request);

    if (result && result->data().size()) {
        return result->data().first()->secret();
    }

    return {};
}

QByteArray DBv1::getSecret(const QByteArray &userKey) const {
    auto user = getUser(userKey);
    if (!user) {
        return {};
    }

    return user->secret();
}

QSharedPointer<DP::iPill> DBv1::initPills(const QString& piilId) {

    if (piilId == "InvalidCardIdPill") {
        return QSharedPointer<RC::InvalidCardIdPill>::create(this).dynamicCast<DP::iPill>();
    } else if (piilId == "InvalidUsersPill") {
        return QSharedPointer<RC::InvalidUsersPill>::create(db()).dynamicCast<DP::iPill>();
    }

    return nullptr;
}

QList<QSharedPointer<DP::iPill>> DBv1::initPills() {
    return {
        QSharedPointer<RC::InvalidCardIdPill>::create(this).dynamicCast<DP::iPill>(),
                QSharedPointer<RC::InvalidUsersPill>::create(db()).dynamicCast<DP::iPill>(),
    };
}

bool DBv1::clearOldData(int duration) {
    if (!db())
        return false;

    int timeLine = static_cast<int>(::time(0)) - duration;

    QStringList tables = {
        "Cards",
        "Users",
        "UsersCards"
    };
    for (const QString& table : qAsConst(tables)) {

        QString query = "DELETE FROM %0 WHERE time < '%1'";
        query = query.arg(table).arg(timeLine);

        if (!db()->doQuery(query)) {
            QuasarAppUtils::Params::log(query + " is Failed",
                                        QuasarAppUtils::Error);
            return false;
        }
    }

    QuasarAppUtils::Params::log("The Clear old data finished successfull",
                                QuasarAppUtils::Info);

    return true;

}

QString DBv1::backUp(const QString &path) const {
    return DB::AppDataBase::backUp(0, path);
}
}
