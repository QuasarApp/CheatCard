//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardservice.h"
#include <heart.h>
#include <QDateTime>
#include <db.h>
#include <CheatCard/server.h>
#include <rci/rcutils.h>


CheatCardService::CheatCardService(int argc, char **argv):
    Patronum::Service<QCoreApplication>(argc, argv) {

    QString fileLog = QuasarAppUtils::Params::getArg("fileLog");

    if (!fileLog.length()) {
        QuasarAppUtils::Params::setArg("fileLog",
                                       QStandardPaths::writableLocation(
                                           QStandardPaths::AppDataLocation) + "/log" +
                                       QDateTime::currentDateTimeUtc().toString("_dd_MM_yyyy") +
                                       ".log");
    }

    QuasarAppUtils::Params::log("BackUps available in: " + QuasarAppUtils::Params::getArg("dbBackUp"));
}

CheatCardService::~CheatCardService() {

    if (_server) {
        _server->softDelete();
        _server = nullptr;
    }
}

bool CheatCardService::onStart() {
    srand(time(0));

    if (!_db) {
        _db = RC::DB::makeDb(1,"",
                             QStandardPaths::writableLocation(
                                 QStandardPaths::AppDataLocation) + "/dbBackUp/");

        if (!_db->init()) {
            return false;
        }
    }

    if (!_server) {
        _server = new RC::Server(_db, {3});
    }

    if (!_server->run({}, DEFAULT_CHEAT_CARD_PORT)) {
        QuasarAppUtils::Params::log("Failed to start ssl server!");
        return false;
    }

    return true;
}

bool CheatCardService::handleReceive(const Patronum::Feature &data) {

    if (!_server) {
        sendResuylt("Service is running but server is offline. Send start comand");
        return true;
    }

    if (data.cmd() == "ping") {
        sendResuylt("Pong");
    } else if (data.cmd() == "state") {
        QVariantMap result;
        result["01. Status"] = _server->getWorkState().toString();
        result["02. Log file available"] = QuasarAppUtils::Params::getArg("fileLog", "Not used");
        result["03. Core lib version"] = _server->libVersion();
        result["04. Heart lib version"] = QH::heartLibVersion();
        result["05. Patronum lib version"] = Patronum::patronumLibVersion();

        sendResuylt(result);
    } else if (data.cmd() == "setVerbose") {
        QuasarAppUtils::Params::setArg("verbose", data.arg());

        sendResuylt("New verbose level is " + QuasarAppUtils::Params::getArg("verbose"));
    } else if (data.cmd() == "backUp") {

        if (!_db) {
            sendResuylt("Failed to make backup of data base: Server not initialised");
            return true;
        }

        QString distPath = data.arg();
        QString result = _db->backUp(distPath);
        if (result.size()) {
            sendResuylt("Back up created sucessfull in: " + result);
        } else {
            sendResuylt("Failed to make backup of data base");
        }
    } else if (data.cmd() == "statistic") {
        auto card = QByteArray::fromBase64(data.arg().toLatin1());
         sendResuylt(prepareStatistic(card));

    } else if (data.cmd() == "cardsList") {
        auto user = QByteArray::fromBase64(data.arg().toLatin1());
        sendResuylt(cardList(user));
    }

    return true;
}

QSet<Patronum::Feature> CheatCardService::supportedFeatures() {
    QSet<Patronum::Feature> data;
    data << Patronum::Feature("ping", {}, "This is description of the ping command");
    data << Patronum::Feature("statistic", "card id", "Show the list of card's clients");
    data << Patronum::Feature("cardsList", "user id", "Show list of clients of the card. skip user id for show all cards");
    data << Patronum::Feature("state", {}, "return state");
    data << Patronum::Feature("backUp", "path to backup dir", "make back up of current database ");
    data << Patronum::Feature("setVerbose", "verbose level", "sets new verbose log level");

    return data;
}

void CheatCardService::onResume() {
    onStart();
}

void CheatCardService::onPause() {
    _server->stop();

    sendResuylt("Server stopped successful. (paused)");
}

QVariantMap CheatCardService::prepareStatistic(const QByteArray& card) const {

    QVariantMap result;

    const auto users = _db->getAllUserFromCard(card);

    for (int i = 0; i < users.size(); ++i) {
        if (auto user = users[i]) {
            result[QString::number(i)] = user->toString();
        }
    }

    return result;
}

QVariantMap CheatCardService::cardList(const QByteArray& user) const {
    QVariantMap result;
    QList<QSharedPointer<RC::Interfaces::iCard>> cards;
    if (user.size()) {
        auto userObj = _db->getUser(user);

        if (!userObj) {
            result ["Result"] = "Fail to find user with id : " + user.toBase64();
            return result;
        }

        auto masterKeys = _db->getMasterKeys(userObj->getKey());
        cards = _db->getAllUserOwnCards(userObj->getKey(), masterKeys);

        for (int i = 0; i < cards.size(); i++) {
            if (auto card = cards[i]) {
                result[QString::number(i)] = card->toString();
            }
        }

        return result;

    }

    cards = _db->getAllCards();
    result["User key"] = " Card id card name";
    for (int i = 0; i < cards.size(); i++) {
        if (auto card = cards[i]) {
            QString cardShortString = QString("card id: %0 title: %1").
                                      arg(card->cardId().toBase64(), card->title());
            result[card->ownerSignature().toBase64()] = cardShortString;
        }
    }

    return result;
}
