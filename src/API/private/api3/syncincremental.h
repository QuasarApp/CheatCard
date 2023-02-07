//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef SYNCINCREMENTAL_H
#define SYNCINCREMENTAL_H

#include <abstractdata.h>
#include "datapack.h"

namespace RC {

namespace API {
namespace V3 {


class UsersCards;
class Contacts;

/**
 * @brief The SyncIncremental class This class contains incremental changes of the users data.
 */
class SyncIncremental: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(API::V3::SyncIncremental)

public:
    SyncIncremental();
    bool isValid() const override;

    QH::PKG::DataPack<API::V3::UsersCards> usersCardsToAdd() const;
    void setUsersCardsToAdd(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToAdd);

    QH::PKG::DataPack<API::V3::UsersCards> usersCardsToRemove() const;
    void setUsersCardsToRemove(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToRemove);
    QH::PKG::DataPack<API::V3::Contacts> contactsToRemove() const;
    void setContactsToRemove(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToRemove);
    QH::PKG::DataPack<API::V3::Contacts> contactsToAdd() const;
    void setContactsToAdd(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToAdd);
    const QByteArray &syncedUserKey() const;
    void setSyncedUserKey(const QByteArray &newSyncedUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QH::PKG::DataPack<API::V3::UsersCards> _usersCardsToAdd;
    QH::PKG::DataPack<API::V3::UsersCards> _usersCardsToRemove;


    QH::PKG::DataPack<API::V3::Contacts> _contactsToRemove;
    QH::PKG::DataPack<API::V3::Contacts> _contactsToAdd;

    QByteArray _syncedUserKey;

};

}
}
}
#endif // SYNCINCREMENTAL_H
