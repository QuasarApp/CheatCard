//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "invalidcardidpill.h"
#include "isqldb.h"

#include <QSqlQuery>
#include <sqldbwriter.h>

#include <dbv1.h>

#include <objects/card.h>

namespace RC {

InvalidCardIdPill::InvalidCardIdPill(DBv1 *db) {
    _db = db;
}

QString InvalidCardIdPill::name() const {
    return QObject::tr("Invalid ids of cards");
}

QString InvalidCardIdPill::description() const {
    return QObject::tr("You database have cards that saved with invalid id."
                       " See https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/386 ");
}

int InvalidCardIdPill::id() const {
    return qHash("InvalidCardIdPill");
}

bool InvalidCardIdPill::diagnostic() {

    if (!(_db && _db->db() && _db->db()->writer()))
        return false;

    QString query("Select id from Cards");

    QSqlQuery result;
    if (!_db->db()->doQuery(query, {}, true, &result)) {
        return false;
    }

    while (result.next()) {
        if (result.value(0).toLongLong() < 0) {
            return true;
        }
    }

    return false;
}

bool InvalidCardIdPill::fix() {

    const auto availableCards = _db->getAllUserCards("all", true, {});
    if (availableCards.isEmpty())
        return false;

    if (!_db->db()->doQuery("DELETE FROM Cards WHERE id!='all'", {}, true)) {
        return false;
    }

    bool result = true;
    for (const auto &obj : availableCards) {
        auto card = obj.dynamicCast<DB::Card>();

        if (card) {
            result = result && _db->db()->insertObject(card, true);
        }
    }

    return result;
}

}
