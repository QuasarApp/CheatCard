//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "clearolddata.h"
#include "server.h"
#include <ctime>

namespace RC {

ClearOldData::ClearOldData(int duration) {
    _duration = duration;
}

bool ClearOldData::execute(QH::AbstractNode *node) const {
    auto server = dynamic_cast<BaseNode*>(node);

    if (!server)
        return false;

    auto db = server->db();

    if (!db)
        return false;

    int timeLine = static_cast<int>(::time(0)) - _duration;

    QStringList tables = {
        "Cards",
        "Users",
        "UsersCards",
        "Sessions"
    };
    for (const QString& table : qAsConst(tables)) {

        QString query = "DELETE FROM %0 WHERE time < '%1'";
        query = query.arg(table).arg(timeLine);

        if (!db->doQuery(query)) {
            QuasarAppUtils::Params::log(query + " is Failed",
                                        QuasarAppUtils::Error);
            return false;
        }
    }

    QuasarAppUtils::Params::log("The Clear old data finished successfull",
                                QuasarAppUtils::Info);

    return true;
};

}
