//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#




#ifndef DBv0_USERSCARDS_H
#define DBv0_USERSCARDS_H

#include <rci/objects/iuserscards.h>
#include <dbobject.h>

namespace RC {
namespace DBv0 {

/**
 * @brief The UsersCards class is Database implementation of the iUsersCards interface
 */
class UsersCards: public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    UsersCards();

    unsigned int getUser() const ;
    void setUser(unsigned int newUser) ;
    unsigned int getCard() const ;
    void setCard(unsigned int newCard) ;
    unsigned int getPurchasesNumber() const ;
    void setPurchasesNumber(unsigned int newPurchasesNumber) ;
    unsigned int getReceived() const ;
    void setReceived(unsigned int newReceived) ;
    void receive(unsigned int receiveFreItemsCount) ;
    long long getId() const ;
    unsigned int getCardVersion() const ;
    void setCardVersion(unsigned int newCardVersion) ;
    unsigned int getRawTime() const ;
    bool isActive() const ;
    void setTime(unsigned int newTime) ;

    QH::PKG::DBObject *createDBObject() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    QString table() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    QString toString() const override;
    bool isValid() const override;
protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString primaryKey() const override;
    QVariant primaryValue() const override;

private:
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


#endif // DB_USERSCARDS_H
