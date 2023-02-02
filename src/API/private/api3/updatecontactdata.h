//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef UPDATECONTACTDATA_V3_H
#define UPDATECONTACTDATA_V3_H

#include "rci/objects/icontacts.h"
#include "contacts.h"
#include <packages/abstractdata.h>

namespace RC {
namespace API {
namespace V3 {
/**
 * @brief The UpdateContactData class sendt to server new permssion data.
 */
class UpdateContactData: public Contacts
{
    QH_PACKAGE_AUTO(API::V3::UpdateContactData)

public:
    UpdateContactData();
    UpdateContactData(const Interfaces::iContacts& contact);
    bool isValid() const override;

    const QByteArray &userSecreet() const;
    void setUserSecreet(const QByteArray &newUserSecreet);
    // StreamBase interface
    bool getRemove() const;
    void setRemove(bool newRemove);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray _userSecreet;
    bool remove = false;
};
}
}
}
#endif // UPDATECONTACTDATA_H
