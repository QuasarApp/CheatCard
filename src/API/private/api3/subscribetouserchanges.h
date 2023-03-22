//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef SUBSCRIBETOUSERCHANGES_H
#define SUBSCRIBETOUSERCHANGES_H

#include "abstractdata.h"
namespace RC {
namespace API {
namespace V3 {

/**
 * @brief The SubscribeToUserChanges class is command that can subscribe to changes of the users
 */
class SubscribeToUserChanges: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(RC::API::V3::SubscribeToUserChanges)

public:
    SubscribeToUserChanges();
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
#endif // SUBSCRIBETOUSERCHANGES_H
