//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "invaliduserspill.h"
#include "objects/user.h"
#include <dbv1.h>
#include <QSqlQuery>
#include <dbobjectsrequest.h>
#include <sqldbwriter.h>

namespace RC {

InvalidUsersPill::InvalidUsersPill(const QSharedPointer<Interfaces::iDB> &db) {
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

bool InvalidUsersPill::doFix() {
    if (diagnostic()) {
        return fix();
    }
    return true;
}

bool InvalidUsersPill::diagnostic() {

    if (!_db )
        return false;

    auto users = _db->getAllUserWithPrivateKeys();
    if (users.isEmpty())
        return false;

    for (const auto &i : users) {
        if (!(i->isValid() && i->isAllKeysIsValid())) {
            _brokenUsers.push_back(i);
        }
    }

    return _brokenUsers.size();
}

bool InvalidUsersPill::fix() {

    bool result = true;
    for (const auto &obj : qAsConst(_brokenUsers)) {

        auto user = obj.dynamicCast<DB::User>();

        if (user && user->isValid()) {
            user->setSecret({});
            result = result && _db->saveUser(user);
        } else {
            result = result && _db->deleteUser(user->getKey());
        }
    }

    _brokenUsers.clear();

    return result;
}
}
