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

#include <CheatCard/api/api0/card.h>

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

    result += beta1Patches();

    return result;
}

QH::DBPatchMap DataBase::beta1Patches() const {
    QH::DBPatchMap result;

    // See task #130 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/130
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_1.sql");
    };

    // See task #111 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/111
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_2.sql");
    };

    // See task #201 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201
    // This patch is very hard... and do not change this code.
    result += [](const QH::iObjectProvider* database) -> bool {

        // get a database object for change it
        auto db = const_cast<QH::iObjectProvider*>(database);

        // get all users for fix data.
        QH::PKG::CustomDBRequest<API::User> usersrequest("SELECT * FROM Users");

        // reserve result objects containers
        QList<QSharedPointer<QH::PKG::DBObject>> users;
        QList<QSharedPointer<API::Card>> cards;
        QMap<unsigned int, unsigned int> remapConfig;

        // get users
        if (!db->getAllObjects(usersrequest, users))
            return false;

        for (const auto &obj: qAsConst(users)) {
            // for each users check owned cards list
            // if users have owned cards then they is sellers ...
            // next step is get all owned cards and fix them owners sigantures...
            auto ptr = obj.dynamicCast<API::User>();

            if (!ptr)
                return false;

            // get list of owned cards
            // this code true only for beta 0 version
            QString dbrequest = QString("SELECT * FROM Cards WHERE id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                    arg(ptr->userId()).
                    arg("true");

            QList<QSharedPointer<QH::PKG::DBObject>> localUserCards;
            QH::PKG::CustomDBRequest<API::Card> request(dbrequest);
            auto result = db->getAllObjects(request, localUserCards);

            if (result && localUserCards.size()) {
                // if user has owned cards then they is seller. and we need to regenerate for them new keys..

                // save old userid for move config to new id that will be generated after invke a  regenerateKeys method.
                unsigned int oldUserId = ptr->userId();

                ptr->regenerateKeys();
                QSettings().setValue(CURRENT_USER, ptr->userId());
                remapConfig.insert(ptr->userId(), oldUserId);

                // sets new own signature for each seller card.
                for (const auto &cardObj : qAsConst(localUserCards)) {
                    auto card = cardObj.dynamicCast<API::Card>();
                    if (!card) {
                        continue;
                    }

                    card->setOwnerSignature(ptr->getKey());
                    cards.push_back(card);
                }

            } else {

                // else just update structure of publick key..
                QH::PKG::GetSingleValue request({"Users", ptr->userId()}, "key");
                auto keyWrapper = db->getObject(request);
                QByteArray key = keyWrapper->value().toByteArray();

                if (ptr->userId() != API::User::makeId(key)) {
                    return false;
                }

                ptr->setKey(key);
            }
        }

        if (!database->doSql(":/DataBase/private/sql/SQLPatch_3.sql")) {
            return false;
        }

        // reinsert updated users.
        for (const auto &ptr: qAsConst(users)) {
            // delete old user for refactoring data.
            database->doQuery(QString("DELETE FROM Users WHERE id = '%0'").arg(ptr->getId().toUInt()));

            if (!db->insertObject(ptr, true)) {
                return false;
            }

            unsigned int userId = ptr->getId().toUInt();

            if (remapConfig.contains(userId)) {
                unsigned int oldUserId = remapConfig.value(userId);
                // remap old config for new users
                QString updateConfigRequest = QString("INSERT INTO Config(user,fFirst,fSellerMode) VALUES("
                                                      "'%0',"
                                                      "(SELECT fFirst FROM Config WHERE user='%1'),"
                                                      "(SELECT fSellerMode FROM Config WHERE user='%1'))").
                                              arg(userId).arg(oldUserId);

                if (!database->doQuery(updateConfigRequest)) {
                    return false;
                }
            }
        }

        // update cards
        for (const auto &ptr: qAsConst(cards)) {
            if (!db->insertIfExistsUpdateObject(ptr, true)) {
                return false;
            }
        }

        return true;
    };

    return result;
}
}
