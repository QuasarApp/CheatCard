//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CLEAROLDDATA_H
#define CLEAROLDDATA_H

#include "abstracttask.h"
#include "core_global.h"

namespace RC {

/**
 * @brief The ClearOldData class This pacakge will clear all old data from database.
 */
class CHEATCARD_CORE_EXPORT ClearOldData: public QH::AbstractTask
{
public:
    /**
     * @brief The TimeValue enum
     */
    enum TimeValue {
        Sec = 1,
        Min = 60 * Sec,
        Hour = 60 * Min,
        Day = 24 * Hour
    };

    ClearOldData(int duration = 30 * Day);

    bool execute(QH::AbstractNode *node) const;
private:
    int _duration = 0;
};

}
#endif // CLEAROLDDATA_H
