
//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef RESTORERESPONCE_H
#define RESTORERESPONCE_H
#include "datapack.h"
#include <abstractdata.h>

namespace RC {

namespace API {
    class Contacts;
}

namespace APIv1_5 {
class UsersCards;

/**
 * @brief The GetConntactsRequest class
 */
class RestoreResponce: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(APIv1_5::RestoreResponce)

public:
    RestoreResponce();
    bool isValid() const override;

    const QH::PKG::DataPack<APIv1_5::UsersCards> &usersCards() const;
    void setUsersCards(const QH::PKG::DataPack<APIv1_5::UsersCards> &newUsersCards);

    const QH::PKG::DataPack<API::Contacts> &contacts() const;
    void setContacts(const QH::PKG::DataPack<API::Contacts> &newContacts);

    const QByteArray &userKey() const;
    void setUserKey(const QByteArray &newUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QH::PKG::DataPack<APIv1_5::UsersCards> _usersCards;
    QH::PKG::DataPack<API::Contacts> _contacts;
    QByteArray _userKey;

};

}
}
#endif // RESTORERESPONCE_H
