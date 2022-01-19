//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "invalidcardidpill.h"

#include <CheatCard/api/apiobjectsfactoryv1.h>
#include "CheatCard/api/api1/card.h"
#include <QSqlQuery>
#include <sqldbwriter.h>

namespace RC {

InvalidCardIdPill::InvalidCardIdPill(QH::ISqlDBCache *db) {
    _db = db;
}

QString InvalidCardIdPill::name() const {
    return QObject::tr("Invalid ids of cards");
}

QString InvalidCardIdPill::description() const {
    return QObject::tr("You database have cards that saved with invalid id."
                       " ");
}

bool InvalidCardIdPill::diagnostic() const {

    if (!(_db && _db->writer()))
        return false;

    QString query("Select id from Cards");

    QSqlQuery result;
    if (!_db->doQuery(query, true, &result)) {
        return false;
    }

    while (result.next()) {
        if (result.value(0).toLongLong() < 0) {
            return true;
        }
    }

    return false;
}

bool InvalidCardIdPill::fix() const {
    APIObjectsFactoryV1 factory(_db);

    const auto availableCards = factory.getAllUserCards("all", true);
    if (availableCards.isEmpty())
        return false;

    if (!_db->doQuery("DELETE FROM Cards WHERE id!='all'", true)) {
        return false;
    }

    bool result = true;
    for (const auto &card : availableCards) {
        result = result && _db->insertObject(card, true);
    }

    return result;
}

}
