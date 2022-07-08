
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APIOBJECTSFACTORYV1_H
#define APIOBJECTSFACTORYV1_H

#include "apiobjectsfactoryv0.h"


namespace RC {

class APIObjectsFactoryV1: public APIObjectsFactoryV0
{
public:
    APIObjectsFactoryV1(QH::ISqlDBCache *db);

    QList<QSharedPointer<API::UsersCards> > getAllUserData(unsigned int userId) const override;
    QSharedPointer<API::UsersCards> getUserCardData(unsigned int userId, unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllUserFromCard(unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllPassiveUserFromCard(unsigned int cardId,
                                                                      int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QList<QSharedPointer<API::UsersCards> > getAllActiveUserFromCard(unsigned int cardId,
                                                                     int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QSharedPointer<API::Card> getCard(unsigned int cardId) override;
    QList<QSharedPointer<API::Card> > getAllUserCards(const QByteArray &userKey, bool restOf,
                                                      const QList<QSharedPointer<API::Contacts>> &childs) override;
    QList<QSharedPointer<API::UsersCards> > getAllUserCardsData(const QByteArray &userKey,
                                                                const QList<QSharedPointer<API::Contacts> > &childs) override;

};
}
#endif // IAPIOBJECTSFACTORYV1_H
