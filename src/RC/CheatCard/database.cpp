//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "database.h"
#include "dbobjectsrequest.h"
#include "CheatCard/api/api0/user.h"

#include <QSettings>
#include <customdbrequest.h>
#include <getsinglevalue.h>

namespace RC {

DataBase::DataBase(const QString &name) {
    if (name.size())
        setLocalNodeName(name);
}

QH::ISqlDBCache *DataBase::db() const {
    return QH::DataBaseNode::db();
}

QString DataBase::localFilePath() const {
    return dbLocation();
}

QStringList DataBase::SQLSources() const {
    return QH::DataBaseNode::SQLSources() << ":/DataBase/private/sql/DataBase.sql";
}

// See https://quasarapp.ddns.net:3031/docs/QuasarApp/Heart/latest/classQH_1_1DataBaseNode.html#a9e2969af3bd4e6b49b80820000aef108
QH::DBPatchMap DataBase::dbPatches() const {


    QH::DBPatchMap result;

    result += [](const QH::iObjectProvider* database) -> bool {
        // Some code for update from 0 to 1
        // Example: return database->doSql(patch.sql);
        Q_UNUSED(database);
        return true;
    };

    // See task #130 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/130
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_1.sql");
    };

    // See task #111 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/111
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_2.sql");
    };

    // See task #201 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201
    result += [](const QH::iObjectProvider* database) -> bool {

        QH::PKG::CustomDBRequest<User> request("SELECT * FROM Users");

        auto db = const_cast<QH::iObjectProvider*>(database);
        QList<QSharedPointer<QH::PKG::DBObject>> result;
        if (!db->getAllObjects(request, result))
            return false;

        for (const auto &obj: qAsConst(result)) {

            auto ptr = obj.dynamicCast<User>();

            if (!ptr)
                return false;

            if (ptr->fSaller()) {
                database->doQuery(QString("DELETE FROM Users WHERE id = '%0'").arg(ptr->userId()));
                ptr->regenerateKeys();
                QSettings().setValue(CURRENT_USER, ptr->userId());
            } else {
                QH::PKG::GetSingleValue request({"Users", ptr->userId()}, "key");
                auto keyWrapper = db->getObject(request);
                QByteArray key = keyWrapper->value().toByteArray();

                if (ptr->userId() != User::makeId(key)) {
                    return false;
                }

                ptr->setKey(key);
            }
        }

        if (!database->doSql(":/DataBase/private/sql/SQLPatch_3.sql")) {
            return false;
        }

        for (const auto &ptr: qAsConst(result)) {
            if (!db->insertObject(ptr, true)) {
                return false;
            }
        }

        return true;
    };

    return result;
}
}
