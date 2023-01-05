//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef STATUSAFTERCHANGES_H
#define STATUSAFTERCHANGES_H


#include "api2/userscards.h"
#include <datapack.h>

#include <api0/session.h>

namespace RC {

namespace API {
namespace V2 {


/**
 * @brief The StatusAfterChanges class contains result of the changes.
 */
class StatusAfterChanges: public API::V0::Session
{
    QH_PACKAGE(StatusAfterChanges, "APIv2::StatusAfterChanges")

public:
    StatusAfterChanges();
    ~StatusAfterChanges() override;

    bool status() const;
    void setStatus(bool newStatus);

    // StreamBase interface
    bool neededCard() const;
    void setNeededCard(bool newNeededCard);

    unsigned int getUser() const;
    unsigned int getCard() const;

    const QH::PKG::DataPack<API::V2::UsersCards> &getLastStatus() const;
    void setLastStatus(const QH::PKG::DataPack<API::V2::UsersCards> &newLastStatus);
    void addLastStatus(const QSharedPointer<RC::API::V2::UsersCards> &newLastStatus);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    bool _status = false;
    bool _neededCard = false;
    QH::PKG::DataPack<API::V2::UsersCards> lastStatus;

};
}
}
}
#endif // STATUSAFTERCHANGES_H
