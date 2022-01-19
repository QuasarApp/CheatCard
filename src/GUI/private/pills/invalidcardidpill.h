//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef INVALIDCARDIDPILL_H
#define INVALIDCARDIDPILL_H

#include "doctorpill.h"

namespace QH {
class ISqlDBCache;
}

namespace RC {

class InvalidCardIdPill: public DP::iPill
{

public:
    InvalidCardIdPill(QH::ISqlDBCache* db);

    // iPill interface
public:
    QString name() const override;
    QString description() const override;

protected:
    bool diagnostic() const override;
    bool fix() const override;

private:

    QH::ISqlDBCache * _db = nullptr;
};

}
#endif // INVALIDCARDIDPILL_H
