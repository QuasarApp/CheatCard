
//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APIOBJECTSFACTORYV1_5_H
#define APIOBJECTSFACTORYV1_5_H

#include "apiobjectsfactoryv1.h"


namespace RC {

class APIObjectsFactoryV1_5: public APIObjectsFactoryV1
{
public:
    APIObjectsFactoryV1_5(QH::ISqlDBCache *db);

    QList<QSharedPointer<API::UsersCards> > getAllUserData(unsigned int userId) const override;
    QSharedPointer<API::UsersCards> getUserCardData(unsigned int userId, unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllUserFromCard(unsigned int cardId) const override;
    QList<QSharedPointer<API::UsersCards> > getAllPassiveUserFromCard(unsigned int cardId,
                                                                      int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QList<QSharedPointer<API::UsersCards> > getAllActiveUserFromCard(unsigned int cardId,
                                                                     int unixTimeRange = ACTIVE_USER_TIME_LIMIT) const override;
    QList<QSharedPointer<API::UsersCards> > getAllUserCardsData(const QByteArray &userKey,
                                                                const QList<QSharedPointer<API::Contacts> > &childs) override;

};
}
#endif // IAPIOBJECTSFACTORYV1_H
