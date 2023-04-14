//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#




#ifndef DB_USERSCARDS_H
#define DB_USERSCARDS_H

#include <rci/objects/iuserscards.h>
#include <dbobject.h>

namespace RC {
namespace DB {

/**
 * @brief The UsersCards class is Database implementation of the iUsersCards interface
 */
class UsersCards: public RC::Interfaces::iUsersCards, public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    UsersCards();
    UsersCards(const QByteArray &user, const QByteArray &card);
    UsersCards(const QSharedPointer<Interfaces::iUsersCards> &obj);

    const QByteArray& getUser() const override;
    void setUser(const QByteArray& newUser) override;
    const QByteArray& getCard() const override;
    void setCard(const QByteArray& newCard) override;
    unsigned int getPurchasesNumber() const override;
    void setPurchasesNumber(unsigned int newPurchasesNumber) override;
    unsigned int getReceived() const override;
    void setReceived(unsigned int newReceived) override;
    void receive(unsigned int receiveFreItemsCount) override;
    unsigned int getCardVersion() const override;
    void setCardVersion(unsigned int newCardVersion) override;
    unsigned int getRawTime() const override;
    bool isActive() const override;
    void setTime(unsigned int newTime) override;

    QH::PKG::DBObject *createDBObject() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    QString table() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    QString toString() const override;
    bool isValid() const override;
    std::pair<QString, QMap<QString, QVariant> > condition() const override;

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


#endif // DB_USERSCARDS_H
