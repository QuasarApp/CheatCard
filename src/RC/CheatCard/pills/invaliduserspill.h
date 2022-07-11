//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef INVALIDUSERSPILL_H
#define INVALIDUSERSPILL_H


#include "doctorpill.h"
#include "CheatCard/core_global.h"

#include <QSharedPointer>

namespace QH {
class ISqlDBCache;
}

namespace RC {

namespace API {
class User;
};

/**
 * @brief The InvalidUsersPill class sets to empty private key if the private key of user is not master key of public user's key
 */
class CHEATCARD_CORE_EXPORT InvalidUsersPill: public DP::iPill
{
public:
    InvalidUsersPill(QH::ISqlDBCache* db);

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
    QList<QSharedPointer<API::User>> _brokenUsers;
    QH::ISqlDBCache * _db = nullptr;


};
}
#endif // INVALIDUSERSPILL_H
