//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_V3_H
#define USERSCARDS_V3_H

#include "idataconvertor.h"
#include "rci/objects/iuserscards.h"
#include <packages/abstractdata.h>

namespace RC {
namespace API {
namespace V3 {

class UsersCards: public QH::PKG::AbstractData, public iDataConvertor<RC::Interfaces::iUsersCards>
{
    QH_PACKAGE_AUTO(RC::API::V3::UsersCards)

public:
    UsersCards();
    UsersCards(const QByteArray& user, const QByteArray& card);
    UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj);


    QByteArray getUser() const;
    void setUser(const QByteArray &newUser);
    QByteArray getCard() const;
    void setCard(const QByteArray &newCard);
    unsigned int getCardVersion() const;
    void setCardVersion(unsigned int newCardVersion);
    unsigned int getPurchasesNumber() const;
    void setPurchasesNumber(unsigned int newPurchasesNumber);
    unsigned int getReceived() const;
    void setReceived(unsigned int newReceived);
    unsigned int time() const;
    void setTime(unsigned int newTime);

    bool isValid() const override;

    virtual QSharedPointer<RC::Interfaces::iUsersCards>
    toObject(const QSharedPointer<Interfaces::iDB> &db) const override;
    void fromObject(const QSharedPointer<RC::Interfaces::iUsersCards>& obj) override;


protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray user;
    QByteArray card;
    unsigned int cardVersion = 0;
    unsigned int purchasesNumber = 0;
    unsigned int received = 0;
    unsigned int _time;
};

}
}
}

#endif // USERSCARDS_V1_H
