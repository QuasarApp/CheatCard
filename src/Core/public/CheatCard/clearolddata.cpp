//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "clearolddata.h"
#include "params.h"
#include "rci/core/idb.h"
#include <ctime>

namespace RC {

ClearOldData::ClearOldData(QSharedPointer<Interfaces::iDB> db, int duration) {
    _duration = duration;
    _db = db;
}

bool ClearOldData::isValid() const {
    return _db;
}

bool ClearOldData::execute(QH::AbstractNode *) const {
    if (!_db)
        return false;

    if (!_db->clearOldData(_duration)) {
        QuasarAppUtils::Params::log("ClearOldData action is Failed",
                                    QuasarAppUtils::Error);
        return false;
    }

    QuasarAppUtils::Params::log("The Clear old data finished successfull",
                                QuasarAppUtils::Info);

    return true;
};

}
