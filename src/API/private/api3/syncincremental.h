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

    const QH::PKG::DataPack<API::V3::UsersCards>& usersCardsToAdd() const;
    void setUsersCardsToAdd(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToAdd);
    void addUsersCardsToAdd(const QSharedPointer<API::V3::UsersCards> &userData);

    const QH::PKG::DataPack<API::V3::UsersCards>& usersCardsToRemove() const;
    void setUsersCardsToRemove(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToRemove);
    void addUsersCardsToRemove(const QSharedPointer<API::V3::UsersCards> &userData);

    const QH::PKG::DataPack<API::V3::Contacts>& contactsToRemove() const;
    void setContactsToRemove(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToRemove);
    void addContactsToRemove(const QSharedPointer<API::V3::Contacts> &newContactsToAdd);

    const QH::PKG::DataPack<API::V3::Contacts>& contactsToAdd() const;
    void setContactsToAdd(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToAdd);
    void addContactsToAdd(const QSharedPointer<API::V3::Contacts> &newContactsToAdd);

    bool getResult() const;
    void setResult(bool newResult);

    QHash<QByteArray, int> cardUpdated() const;
    void setCardUpdated(const QHash<QByteArray, int> &newCardUpdated);
    void addCardToUpdate(const QByteArray& cardId, int version);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QHash<QByteArray, int> _cardUpdated;

    QH::PKG::DataPack<API::V3::UsersCards> _usersCardsToAdd;
    QH::PKG::DataPack<API::V3::UsersCards> _usersCardsToRemove;

    QH::PKG::DataPack<API::V3::Contacts> _contactsToRemove;
    QH::PKG::DataPack<API::V3::Contacts> _contactsToAdd;

    bool result = false;
};

}
}
}
#endif // SYNCINCREMENTAL_H
