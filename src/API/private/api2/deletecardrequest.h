//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DELETECARDREQUEST_H
#define DELETECARDREQUEST_H
#include <abstractdata.h>


namespace RC {
namespace API {
namespace V2 {

/**
 * @brief The DeleteCardRequest class delete the card from server. Onlt owners can delete cards.
 */
class DeleteCardRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE(DeleteCardRequest, "APIv2::DeleteCardRequest")

public:
    DeleteCardRequest();
    bool isValid() const override;

    unsigned int card() const;
    void setCard(unsigned int newCard);

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _card;
    QByteArray _secret;
};
}
}
}
#endif // DELETECARDREQUEST_H
