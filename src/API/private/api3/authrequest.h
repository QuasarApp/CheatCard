//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef AUTHREQUEST_H
#define AUTHREQUEST_H

#include "abstractdata.h"
namespace RC {
namespace API {
namespace V3 {

/**
 * @brief The AuthRequest class is command that can subscribe to changes of the users
 */
class AuthRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(RC::API::V3::AuthRequest)

public:
    AuthRequest();
    bool isValid() const override;

    const QByteArray& userId() const;
    void setUserId(const QByteArray &newUserId);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray _userId;
};

}
}
}
#endif // AUTHREQUEST_H
