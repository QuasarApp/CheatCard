//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "appdatabase.h"
#include "objects/contacts.h"
#include "objects/userscards.h"
#include <sqldbwriter.h>

#include <customdbrequest.h>
#include <getsinglevalue.h>

#include <QDateTime>
#include <QDir>

#include <pills/invaliduserspill.h>

#include <objects/card.h>
#include <objects/user.h>

#include <rci/rcutils.h>

#include <dbobjectsrequest.h>

#include <objects_v0/card.h>
#include <objects_v0/user.h>
#include <objects_v0/userscards.h>
#include <objects_v0/contacts.h>

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

    return QH::DataBase::SQLSources();
}

void AppDataBase::localdbPatches() {

    addDBPatch({
                   0,
                   6,
                   [](QH::iObjectProvider* database) -> bool {
                       return database->doSql(":/DataBase/private/sql/SQLPatch_6.sql");
                   }
               });

    addDBPatch({
                   6,
                   7,
                   [](QH::iObjectProvider* database) -> bool {
                       if (!database->doSql(":/DataBase/private/sql/SQLPatch_7.sql")) {
                           return false;
                       }

                       QHash<unsigned int, QByteArray> usersKeysPairs;
                       {
                           QH::PKG::DBObjectsRequest<DBv0::User> query("Users_old");
                           auto result = database->getObject(query);
                           if (result && result->data().size()) {
                               for (const auto& user: result->data()) {
                                   auto newUser = QSharedPointer<DB::User>::create();
                                   newUser->setKey(user->getKey());
                                   newUser->setSecret(user->secret());
                                   newUser->setFSaller(user->fSaller());
                                   newUser->setName(user->name());

                                   if (!database->insertObject(newUser, true)) {
                                       return false;
                                   }

                                   usersKeysPairs[user->id()] = newUser->getKey();
                               }
                           }
                       }

                       // port of a cards data.
                       {
                           QH::PKG::DBObjectsRequest<DBv0::Card> query("Cards_old");
                           auto result = database->getObject(query);
                           if (result && result->data().size()) {
                               for (const auto& object: result->data()) {
                                   auto newCard = QSharedPointer<DB::Card>::create();
                                   newCard->setCardId(RCUtils::convrtOldIdToSHA256(object->cardId()));
                                   newCard->setTitle(object->title());

                                   newCard->setCardVersion(object->getCardVersion());
                                   newCard->setBackground(object->background());
                                   newCard->setColor(object->getColor());
                                   newCard->setFontColor(object->getFontColor());
                                   newCard->setFreeIndex(object->getFreeIndex());
                                   newCard->setFreeItemName(object->freeItemName());
                                   newCard->setInstagramm(object->instagramm());
                                   newCard->setTelegramm(object->telegramm());
                                   newCard->setLogo(object->logo());
                                   newCard->setOwnerSignature(object->ownerSignature());
                                   newCard->setPhone(object->phone());
                                   newCard->setPhysicalAddress(object->physicalAddress());
                                   newCard->setSeal(object->seal());
                                   newCard->setWebSite(object->webSite());

                                   if (!database->insertObject(newCard, true)) {
                                       return false;
                                   }
                               }
                           }
                       }

                       // port of the contacts table
                       {
                           QH::PKG::DBObjectsRequest<DBv0::Contacts> query("Contacts_old");
                           auto result = database->getObject(query);
                           if (result && result->data().size()) {
                               for (const auto& object: result->data()) {
                                   auto newContact = QSharedPointer<DB::Contacts>::create();
                                   auto childId = object->getChildUserKey();
                                   unsigned int oldChildId = RCUtils::makeOlduserId(childId);
                                   newContact->setChildUserKey(childId);
                                   usersKeysPairs[oldChildId] = childId;

                                   auto masterId = object->getUserKey();
                                   unsigned int oldMasterId = RCUtils::makeOlduserId(masterId);
                                   newContact->setUserKey(masterId);
                                   usersKeysPairs[oldMasterId] = masterId;

                                   newContact->setInfo(object->getInfo());

                                   if (!database->insertObject(newContact, true)) {
                                       return false;
                                   }
                               }
                           }
                       }

                       // port of the usersdata table
                       {
                           QH::PKG::DBObjectsRequest<DBv0::UsersCards> query("UsersCards");
                           auto result = database->getObject(query);
                           if (result && result->data().size()) {
                               for (const auto& object: result->data()) {
                                   auto userKey = usersKeysPairs.value(object->getUser(), 0);

                                   // usualy database do not contains full information about user struct.
                                   // so Current database can't upgrade to latest version.
                                   // Data that can't migrate at now will migrate after receive information about user from client.
                                   if (userKey.isEmpty())
                                       continue;

                                   auto newCard = QSharedPointer<DB::UsersCards>::create();
                                   newCard->setCard(RCUtils::convrtOldIdToSHA256(object->getCard()));
                                   newCard->setUser(userKey);
                                   newCard->setCardVersion(object->getCardVersion());
                                   newCard->setPurchasesNumber(object->getPurchasesNumber());
                                   newCard->setReceived(object->getReceived());
                                   newCard->setTime(object->getRawTime());

                                   if (!database->insertObject(newCard, true)) {
                                       return false;
                                   }

                                   if (!database->deleteObject(object, true)) {
                                       return false;
                                   }
                               }
                           }
                       }

                       return database->doSql(":/DataBase/private/sql/SQLPatch_7_2.sql");
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
