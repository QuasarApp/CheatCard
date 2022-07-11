//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "invaliduserspill.h"
#include <CheatCard/api/apiobjectsfactoryv1.h>
#include "CheatCard/api/api1/card.h"
#include <QSqlQuery>
#include <sqldbwriter.h>
#include <CheatCard/api/api0/user.h>

namespace RC {

InvalidUsersPill::InvalidUsersPill(QH::ISqlDBCache *db) {
    _db = db;
}

QString InvalidUsersPill::name() const {
    return QObject::tr("Invalid users data");
}

QString InvalidUsersPill::description() const {
    return QObject::tr("You database have users with invalid keys."
                       " See https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/539 ");
}

int InvalidUsersPill::id() const {
    return qHash("InvalidUsersPill");
}

bool InvalidUsersPill::diagnostic() {

    if (!(_db && _db->writer()))
        return false;


    QH::PKG::DBObjectsRequest<API::User> request("Users",
                                            "secret != \"\"");

    auto users = _db->getObject(request);

    if (!users)
        return false;

    for (const auto &i : users->data()) {
        if (!(i->isValid() && i->isAllKeysIsValid())) {
            _brokenUsers.push_back(i);
        }
    }

    return _brokenUsers.size();
}

bool InvalidUsersPill::fix() {

    bool result = true;
    for (const auto &user : qAsConst(_brokenUsers)) {

        if (user->isValid()) {
            user->setSecret({});
            result = result && _db->insertObject(user, true);
        } else {
            result = result && _db->deleteObject(user);
        }
    }

    _brokenUsers.clear();

    return result;
}
}
