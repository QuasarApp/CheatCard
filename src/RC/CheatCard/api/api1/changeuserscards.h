//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CHANGEUSERSCARDS_H
#define CHANGEUSERSCARDS_H


#include <CheatCard/api/api0/session.h>

namespace RC {
namespace APIv1 {


/**
 * @brief The ChangeUsersCards class contains changes to server.
 */
class ChangeUsersCards : public API::Session
{
    QH_PACKAGE_AUTO(APIv1::ChangeUsersCards)

public:
    ChangeUsersCards();
    ~ChangeUsersCards() override;

    qint8 receive() const;
    void setReceive(qint8 newReceive);

    qint8 purchase() const;
    void setPurchase(qint8 newPurchase);

    unsigned int getUser() const;
    unsigned int getCard() const;

    bool isValid() const override;

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;


private:
    qint8 _receive = 0;
    qint8 _purchase = 0;
    QByteArray _secret = 0;

};

}
}
#endif // CHANGEUSERSCARDS_H
