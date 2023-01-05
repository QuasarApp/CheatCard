//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_H
#define USERSCARDS_H

#include "rci/core/idb.h"
#include <QDateTime>
#include <QSharedPointer>
#include <abstractdata.h>

#include <rci/objects/iuserscards.h>

namespace RC {
namespace API {
namespace V0 {


class UsersCards: public QH::PKG::AbstractData
{
    QH_PACKAGE(UsersCards, "UsersCards")

public:
    UsersCards();
    UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj);

    UsersCards(unsigned int user, unsigned int card);

public:
    bool isValid() const override;

    unsigned int getUser() const;
    void setUser(unsigned int newUser);

    unsigned int getCard() const;
    void setCard(unsigned int newCard);

    unsigned int getPurchasesNumber() const;
    void setPurchasesNumber(unsigned int newPurchasesNumber);

    unsigned int getReceived() const;
    void setReceived(unsigned int newReceived);
    void receive(unsigned int receiveFreItemsCount);

    long long getId() const;

    unsigned int getCardVersion() const;
    void setCardVersion(unsigned int newCardVersion);

    unsigned int getRawTime() const;
    QDateTime getTime() const;
    bool isActive() const;

    void setTime(unsigned int newTime);

    virtual QSharedPointer<RC::Interfaces::iUsersCards>
    toObject(const QSharedPointer<Interfaces::iDB> &db);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

    long long id = 0;
    unsigned int user = 0;
    unsigned int card = 0;
    unsigned int cardVersion = 0;
    unsigned int purchasesNumber = 0;
    unsigned int received = 0;
    unsigned int _time;
};

}
}
}

#endif // USERSCARDS_H
