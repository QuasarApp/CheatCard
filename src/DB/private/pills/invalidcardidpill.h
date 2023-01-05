//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef INVALIDCARDIDPILL_H
#define INVALIDCARDIDPILL_H

#include "dbv1.h"
#include <DoctorPillCore/ipill.h>

namespace QH {
class ISqlDB;
}

namespace RC {

/**
 * @brief The InvalidCardIdPill class
 * see the https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/386
 */
class InvalidCardIdPill: public DP::iPill
{

public:
    InvalidCardIdPill(DBv1 *db);

    // iPill interface
public:
    QString name() const override;
    QString description() const override;
    int id() const override;

protected:
    bool diagnostic() override;
    bool fix() override;

private:

    DBv1 *_db = nullptr;
};

}
#endif // INVALIDCARDIDPILL_H
