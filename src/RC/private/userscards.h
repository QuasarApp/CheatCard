//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_H
#define USERSCARDS_H

#include <dbobject.h>
#include "CheatCard/CheatCard_global.h"

namespace RC {


class CHEATCARD_EXPORT UsersCards: public QH::PKG::DBObject
{
public:
    UsersCards();
    UsersCards(unsigned int user, unsigned int card, bool owner = false);

public:
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    bool isValid() const override;

    unsigned int getUser() const;
    void setUser(unsigned int newUser);

    unsigned int getCard() const;
    void setCard(unsigned int newCard);

    bool getOwner() const;
    void setOwner(bool newOwner);

    unsigned int getPurchasesNumber() const;
    void setPurchasesNumber(unsigned int newPurchasesNumber);

    unsigned int getReceived() const;
    void setReceived(unsigned int newReceived);
    void receive(unsigned int receiveFreItemsCount);

    static long long genId(unsigned int user, unsigned int card);

    long long getId() const;

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
private:

    long long id = 0;
    unsigned int user = 0;
    unsigned int card = 0;
    unsigned int purchasesNumber = 0;
    unsigned int received = 0;
    bool owner = false;


};

}
#endif // USERSCARDS_H
