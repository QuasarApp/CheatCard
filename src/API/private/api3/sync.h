
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
class Contacts;

/**
 * @brief The GetConntactsRequest class base class for sync information betwin nodes.
 */
class Sync: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(API::V3::Sync)

    public:

        /**
         * @brief The Mode enum is shor description of the node sync
         */
    enum Mode {

        Invalid         = 0x00,

        UsersData       = 0x01,
        Contacts        = 0x02
    };

    Sync();
    bool isValid() const override;

    const QH::PKG::DataPack<API::V3::UsersCards> &usersCards() const;
    void setUsersCards(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCards);

    const QH::PKG::DataPack<API::V3::Contacts> &contacts() const;
    void setContacts(const QH::PKG::DataPack<API::V3::Contacts> &newContacts);
    Mode mode() const;
    void setMode(Mode newMode);

    bool isContainsPermisionsInfo() const;
    bool isContainsUsersDataInfo() const;

    QByteArray syncedUserKey() const;
    void setSyncedUserKey(const QByteArray &newSyncedUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QH::PKG::DataPack<API::V3::UsersCards> _usersCards;
    QH::PKG::DataPack<API::V3::Contacts> _contacts;
    QByteArray _syncedUserKey;

    Mode _mode = Invalid;
};

}
}
}
#endif // RESTORERESPONCE_H
