
//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef RESTORERESPONCE_V3_H
#define RESTORERESPONCE_V3_H

#include "datapack.h"
#include <abstractdata.h>

namespace RC {

namespace API {
namespace V3 {

class UsersCards;

/**
 * @brief The GetConntactsRequest class
 */
class RestoreResponce: public QH::PKG::AbstractData
{
    QH_PACKAGE(RestoreResponce, "APIv3::RestoreResponce")

public:
    RestoreResponce();
    bool isValid() const override;

    const QH::PKG::DataPack<API::V3::UsersCards> &usersCards() const;
    void setUsersCards(const QH::PKG::DataPack<API::V2::UsersCards> &newUsersCards);

    const QH::PKG::DataPack<API::V3::Contacts> &contacts() const;
    void setContacts(const QH::PKG::DataPack<API::V3::Contacts> &newContacts);

    const QByteArray &userKey() const;
    void setUserKey(const QByteArray &newUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QH::PKG::DataPack<API::V3::UsersCards> _usersCards;
    QH::PKG::DataPack<API::V3::Contacts> _contacts;
    QByteArray _userKey;

};

}
}
}
#endif // RESTORERESPONCE_H
