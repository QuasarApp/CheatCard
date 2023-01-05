//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "appdatabase.h"
#include <sqldbwriter.h>

#include <customdbrequest.h>
#include <getsinglevalue.h>

#include <QDateTime>
#include <QDir>

#include <pills/invaliduserspill.h>

#include <objects/card.h>
#include <objects/user.h>
#include "rcdb/settingskeys.h"

#include <rci/rcutils.h>

namespace RC {
namespace DB {

AppDataBase::AppDataBase(const QString &name, const QString &backUpLocation,
                         const QString &customSources) {
    if (name.size())
        setLocalNodeName(name);

    _customSqlSources = customSources;

    setBackUpPath(backUpLocation);
    localdbPatches();
}

QH::ISqlDB *AppDataBase::db() const {
    return QH::DataBase::db();
}

QString AppDataBase::localFilePath() const {
    return dbLocation();
}

QStringList AppDataBase::SQLSources() const {
    if (_customSqlSources.size()) {
        return QH::DataBase::SQLSources() << _customSqlSources;
    }

    return QH::DataBase::SQLSources() << ":/DataBase/private/sql/DataBase.sql";
}

void AppDataBase::localdbPatches() {
    unsigned short dbVersion = 0;

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_1.sql");
        }
    });

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_2.sql");
        }
    });

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            // get a database object for change it
            auto db = const_cast<QH::iObjectProvider*>(database);

            // get all users for fix data.
            QH::PKG::CustomDBRequest<DB::User> usersrequest("SELECT * FROM Users");

            // reserve result objects containers
            QList<QSharedPointer<QH::PKG::DBObject>> users;
            QList<QSharedPointer<DB::Card>> cards;
            QMap<unsigned int, unsigned int> remapConfig;

            // get users
            if (!db->getAllObjects(usersrequest, users))
                return false;

            for (const auto &obj: qAsConst(users)) {
                // for each users check owned cards list
                // if users have owned cards then they is sellers ...
                // next step is get all owned cards and fix them owners sigantures...
                auto ptr = obj.dynamicCast<DB::User>();

                if (!ptr)
                    return false;

                // get list of owned cards
                // this code true only for beta 0 version
                QString dbrequest = QString("SELECT * FROM Cards WHERE id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                                    arg(ptr->id()).
                                    arg("true");

                QList<QSharedPointer<QH::PKG::DBObject>> localUserCards;
                QH::PKG::CustomDBRequest<DB::Card> request(dbrequest);
                auto result = db->getAllObjects(request, localUserCards);

                if (result && localUserCards.size()) {
                    // if user has owned cards then they is seller. and we need to regenerate for them new keys..

                    // save old userid for move config to new id that will be generated after invke a  regenerateKeys method.
                    unsigned int oldUserId = ptr->id();

                    ptr->regenerateKeys();

                    auto settings = QuasarAppUtils::ISettings::instance();
                    settings->setValue(P_CURRENT_USER, ptr->id());

                    remapConfig.insert(ptr->id(), oldUserId);

                    // sets new own signature for each seller card.
                    for (const auto &cardObj : qAsConst(localUserCards)) {
                        auto card = cardObj.dynamicCast<DB::Card>();
                        if (!card) {
                            continue;
                        }

                        card->setOwnerSignature(ptr->getKey());
                        card->setCardVersion(card->getCardVersion() + 1);
                        cards.push_back(card);
                    }

                } else {

                    // else just update structure of publick key..
                    QH::PKG::GetSingleValue request({"Users", ptr->id()}, "key");
                    auto keyWrapper = db->getObject(request);
                    QByteArray key = keyWrapper->value().toByteArray();

                    if (ptr->id() != RC::RCUtils::makeUserId(key)) {
                        return false;
                    }

                    ptr->setKey(key);
                }
            }

            if (!database->doSql(":/DataBase/private/sql/SQLPatch_3.sql")) {
                return false;
            }

            // reinsert updated users.
            for (const auto &rawptr: qAsConst(users)) {

                auto ptr = rawptr.dynamicCast<DB::User>();

                // delete old user for refactoring data.
                database->doQuery(QString("DELETE FROM Users WHERE id = '%0'").arg(ptr->id()));

                if (!db->insertObject(ptr, true)) {
                    return false;
                }

                unsigned int userId = ptr->id();

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
                if (!db->replaceObject(ptr, true)) {
                    return false;
                }
            }

            return true;
        }
    });

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_4.sql");
        }
    });

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            if (!database->doSql(":/DataBase/private/sql/SQLPatch_5.sql"))
                return false;

            auto ISqlDBCache = const_cast<QH::ISqlDB*>(dynamic_cast<const QH::ISqlDB*>(database));
            if (!ISqlDBCache) {
                return false;
            }

            InvalidUsersPill pill(ISqlDBCache);
            return pill.doFix();
        }
    });

    addDBPatch({
        dbVersion,
        ++dbVersion,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_6.sql");
        }
    });
}

void AppDataBase::onBeforeDBUpgrade(int currentVersion, int ) const {
    backUp(currentVersion);
}

QString AppDataBase::backUp(int version, QString path) const {

    if (path.isEmpty()) {
        path = _backUpPath;
    }

    if (path.isEmpty()) {
        return {};
    }

    auto file = path + "/DBv%0_" + QDateTime::currentDateTimeUtc().toString("hh:mm:ss_dd_MM_yyyy") + ".db";
    file = file.arg(version);
    if (db() && db()->writer() &&
            QFile::exists(db()->writer()->databaseLocation())) {

        QDir().mkpath(path);

        if (!QFile::copy(db()->writer()->databaseLocation(), file)) {
            return {};
        }
    }

    return file;
}

const QString &AppDataBase::backUpPath() const {
    return _backUpPath;
}

void AppDataBase::setBackUpPath(const QString &newBackUpPath) {
    _backUpPath = newBackUpPath;
}

}
}
