//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef INVALIDUSERSPILL_H
#define INVALIDUSERSPILL_H


#include "rci/core/idb.h"
#include "rci/objects/iuser.h"
#include <QSharedPointer>

#include <DoctorPillCore/ipill.h>

namespace RC {


/**
 * @brief The InvalidUsersPill class sets to empty private key if the private key of user is not master key of public user's key
 */
class InvalidUsersPill: public DP::iPill
{
public:
    InvalidUsersPill(const QSharedPointer<Interfaces::iDB>& db);

    // iPill interface
public:
    QString name() const override;
    QString description() const override;
    int id() const override;

    bool doFix();

protected:
    bool diagnostic() override;
    bool fix() override;

private:
    QList<QSharedPointer<Interfaces::iUser>> _brokenUsers;
    QSharedPointer<Interfaces::iDB> _db = nullptr;


};
}
#endif // INVALIDUSERSPILL_H
