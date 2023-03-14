
//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DBV1_H
#define DBV1_H

#include "rci/core/idb.h"
#include "appdatabase.h"
 #include <QEnableSharedFromThis>

namespace RC {

namespace DB {
    class UsersCards;
}

class DBv1: public Interfaces::iDB, public DB::AppDataBase, public QEnableSharedFromThis<DBv1>
{
public:
    DBv1(const QString& name = "",
         const QString& backUpLocation = "",
         const QString& custoSql = "");

    bool init() override;
    bool isInit() const override;

    bool deleteContact(const QSharedPointer<Interfaces::iContacts>& contact) const override;
    bool deleteCard(const QByteArray& cardId) const override;
    bool deleteUser(const QByteArray& userId) const override;
    bool deleteContactsByChildUserKey(const QByteArray& childUser) const override;
    bool deleteUserData(const QByteArray &cardId, const QByteArray &userId) override;

    QSharedPointer<Interfaces::iContacts> makeEmptyContact() const override;
    QSharedPointer<Interfaces::iUser> makeEmptyUser() const override;
    QSharedPointer<Interfaces::iCard> makeEmptyCard() const override;
    QSharedPointer<Interfaces::iUsersCards> makeEmptyUsersCard() const override;

    bool saveUser(const QSharedPointer<Interfaces::iUser>& user) const override;
    bool saveCard(const QSharedPointer<Interfaces::iCard>& card) const override;
    bool saveUsersCard(const QSharedPointer<Interfaces::iUsersCards>& userData) const override;
    bool saveContact(const QSharedPointer<Interfaces::iContacts>& contact) const override;

    int getFreeItemsCount(const QSharedPointer<Interfaces::iUsersCards> &inputData) const override;
    int getCountOfReceivedItems(const QByteArray& userId, const QByteArray& cardId) override;
    unsigned int getCardVersion(const QByteArray& cardId) const override;
    QVariant getCardField(const QByteArray& cardId, const QString& field) override;

    QSharedPointer<Interfaces::iUser>
    getUser(const QByteArray &userId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllUserData(const QByteArray& userId) const override;
    QSharedPointer<Interfaces::iUsersCards>
    getUserCardData(const QByteArray &userId, const QByteArray &cardId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllUserFromCard(const QByteArray& cardId, const QByteArray& ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllActiveUserFromCard(const QByteArray& cardId, int unixTimeRange, const QByteArray& ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllPassiveUserFromCard(const QByteArray& cardId, int unixTimeRange, const QByteArray& ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUser> >
    getAllUserDataFromCard(const QByteArray &cardId) const override;
    QList<QSharedPointer<Interfaces::iUser> >
    getAllUserWithPrivateKeys() const override;
    QSharedPointer<Interfaces::iCard>
    getCard(const QByteArray &cardId) override;
    QList<QSharedPointer<Interfaces::iCard> >
    getAllUserCards(const QByteArray &userKey, bool restOf, const QList<QSharedPointer<Interfaces::iContacts> > &childs) override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllUserCardsData(const QByteArray &userKey, const QList<QSharedPointer<Interfaces::iContacts> > &childs) override;
    QSharedPointer<Interfaces::iContacts>
    getContactFromChildId(const QByteArray &userKey, const QByteArray &childUserId) override;
    QList<QSharedPointer<Interfaces::iContacts> >
    getMasterKeys(const QByteArray &childUserId) override;
    QList<QSharedPointer<Interfaces::iContacts> >
    getSlaveKeys(const QByteArray &userId) override;

    QByteArray getSecretOfCardOvner(const QByteArray &cardId) const override;
    QByteArray getSecret(const QByteArray& userKey) const override;

    QSharedPointer<DP::iPill> initPills(const QString &piilId) override;
    QList<QSharedPointer<DP::iPill>> initPills() override;

    bool clearOldData(int duration) override;
    QString backUp(const QString &path) const override;
    bool migrateUsersCardsToUsersData(const QByteArray& userKey) const override;


private:
    int getFreeItemsCount(const QSharedPointer<Interfaces::iUsersCards> &inputData,
                          unsigned int freeIndex) const;

    void prepareOwnerSignatureCondition(const QList<QByteArray> &signatureList,
                                        const QString &operatorName,
                                        QString &where,
                                        QVariantMap &toBind);

    int getCardFreeIndex(const QByteArray& cardId) const;


};
}
#endif // DBV1_H
