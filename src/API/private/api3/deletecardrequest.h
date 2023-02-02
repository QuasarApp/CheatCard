//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DELETECARDREQUEST_V3_H
#define DELETECARDREQUEST_V3_H
#include <abstractdata.h>


namespace RC {
namespace API {
namespace V3 {

/**
 * @brief The DeleteCardRequest class delete the card from server. Onlt owners can delete cards.
 */
class DeleteCardRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE(DeleteCardRequest, "APIv3::DeleteCardRequest")

public:
    DeleteCardRequest();
    bool isValid() const override;

    const QByteArray& card() const;
    void setCard(const QByteArray& newCard);

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray _card;
    QByteArray _secret;
};
}
}
}
#endif // DELETECARDREQUEST_H
