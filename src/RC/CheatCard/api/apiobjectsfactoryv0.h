
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APIOBJECTSFACTORYV0_H
#define APIOBJECTSFACTORYV0_H

#include "apiobjectsfactory.h"


namespace RC {
class APIObjectsFactoryV0 : public APIObjectsFactory
{
public:
    APIObjectsFactoryV0(QH::ISqlDBCache *db);

    // IAPIObjectsFactory interface

    QSharedPointer<API::User> getUser(unsigned int userId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllUserData(unsigned int userId) const override;
    QSharedPointer<API::UsersCards> getUserCardData(unsigned int userId, unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllUserFromCard(unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllPassiveUserFromCard(unsigned int cardId,
                                                                      int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QList<QSharedPointer<API::UsersCards> > getAllActiveUserFromCard(unsigned int cardId,
                                                                     int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QList<QSharedPointer<API::User> > getAllUserDataFromCard(unsigned int cardId) const override;
    QSharedPointer<API::Card> getCard(unsigned int cardId) override;
    QList<QSharedPointer<API::Card> > getAllUserCards(const QByteArray &userKey, bool restOf) override;
    QList<QSharedPointer<API::UsersCards> > getAllUserCardsData(const QByteArray &userKey) override;
    QSharedPointer<API::Contacts> getContactFromChildId(unsigned int userId, unsigned int childUserId) override;
    QSharedPointer<API::Contacts> getContactFromGenesis(unsigned int userId, int genesis) override;
};
}



#endif // APIOBJECTSFACTORYV0_H
