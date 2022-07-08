//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef UPDATECONTACTDATA_H
#define UPDATECONTACTDATA_H

#include <CheatCard/api/api0/contacts.h>

namespace RC {
namespace APIv1_5 {
/**
 * @brief The UpdateContactData class sendt to server new permssion data.
 */
class UpdateContactData: public API::Contacts
{
    QH_PACKAGE_AUTO(APIv1_5::UpdateContactData)

public:
    UpdateContactData();
    UpdateContactData(const API::Contacts& contact);
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
#endif // UPDATECONTACTDATA_H
