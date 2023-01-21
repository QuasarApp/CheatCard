
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


namespace RC {

namespace DB {
    class UsersCards;
}

class DBv1: public Interfaces::iDB, public DB::AppDataBase
{
public:
    DBv1(const QString& name = "",
         const QString& backUpLocation = "",
         const QString& custoSql = "");

    bool init() override;
    bool isInit() const override;

    bool deleteContact(const QSharedPointer<Interfaces::iContacts>& contact) const override;
    bool deleteSessuon(long long sessionId) const override;
    bool deleteCard(unsigned int cardId) const override;
    bool deleteContactsByChildUserKey(const QByteArray& childUser) const override;

    QSharedPointer<Interfaces::iContacts> makeEmptyContact() const override;
    QSharedPointer<Interfaces::iUser> makeEmptyUser() const override;
    QSharedPointer<Interfaces::iCard> makeEmptyCard() const override;
    QSharedPointer<Interfaces::iUsersCards> makeEmptyUsersCard() const override;
    QSharedPointer<Interfaces::iSession> makeEmptySession() const override;

    bool saveUser(const QSharedPointer<Interfaces::iUser>& user) const override;
    bool saveCard(const QSharedPointer<Interfaces::iCard>& card) const override;
    bool saveUsersCard(const QSharedPointer<Interfaces::iUsersCards>& userData) const override;
    bool saveSession(const QSharedPointer<Interfaces::iSession>& session) const override;
    bool saveContact(const QSharedPointer<Interfaces::iContacts>& contact) const override;

    int getFreeItemsCount(const QSharedPointer<Interfaces::iUsersCards> &inputData) const override;
    int getCountOfReceivedItems(unsigned int userId, unsigned int cardId) override;
    unsigned int getCardVersion(unsigned int cardId) const override;
    QVariant getCardField(unsigned int cardId, const QString& field) override;

    QList<QSharedPointer<Interfaces::iUsersCards>>
    getUsersCardsFromSession(long long sessionId) override;
    QSharedPointer<Interfaces::iUser>
    getUser(unsigned int userId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllUserData(unsigned int userId) const override;
    QSharedPointer<Interfaces::iUsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllUserFromCard(unsigned int cardId, unsigned int ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllActiveUserFromCard(unsigned int cardId, int unixTimeRange, unsigned int ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUsersCards> >
    getAllPassiveUserFromCard(unsigned int cardId, int unixTimeRange, unsigned int ignoreUserId) const override;
    QList<QSharedPointer<Interfaces::iUser> >
    getAllUserDataFromCard(unsigned int cardId) const override;
    QList<QSharedPointer<Interfaces::iUser> >
    getAllUserWithPrivateKeys() const override;
    QSharedPointer<Interfaces::iCard>
    getCard(unsigned int cardId) override;
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

    virtual QByteArray getSecretOfCardOvner(unsigned int cardId) const override;
    QByteArray getSecret(const QByteArray& userKey) const override;

    QSharedPointer<DP::iPill> initPills(const QString &piilId) override;
    QList<QSharedPointer<DP::iPill>> initPills() override;

    bool clearOldData(int duration) override;
    QString backUp(const QString &path) const override;


private:
    int getFreeItemsCount(const DB::UsersCards &inputData,
                          unsigned int freeIndex) const;

    int getCardFreeIndex(unsigned int cardId) const;

};
}
#endif // DBV1_H
