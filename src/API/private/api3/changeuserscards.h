//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CHANGEUSERSCARDS_V3_H
#define CHANGEUSERSCARDS_V3_H

#include <packages/abstractdata.h>

namespace RC {
namespace API {
namespace V3 {


/**
 * @brief The ChangeUsersCards class contains changes to server.
 */
class ChangeUsersCards: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(V3::ChangeUsersCards)

public:
    ChangeUsersCards();
    ~ChangeUsersCards() override;

    qint8 receive() const;
    void setReceive(qint8 newReceive);

    qint8 purchase() const;
    void setPurchase(qint8 newPurchase);

    const QByteArray& getUser() const;
    const QByteArray& getCard() const;

    bool isValid() const override;

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

    void setCard(const QByteArray &newCard);

    void setUser(const QByteArray &newUser);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;


private:
    qint8 _receive = 0;
    qint8 _purchase = 0;
    QByteArray _secret;
    QByteArray _card;
    QByteArray _user;


};
}
}
}
#endif // CHANGEUSERSCARDS_H
