//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardservice.h"
#include <QDateTime>

CheatCardService::CheatCardService(int argc, char **argv):
    Patronum::Service<QCoreApplication>(argc, argv) {

    QString fileLog = QuasarAppUtils::Params::getArg("fileLog");

    if (!fileLog.length()) {
        QuasarAppUtils::Params::setArg("fileLog", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/log" + QDateTime::currentDateTimeUtc().toString("_dd_MM_yyyy") + ".log");
    }

    QuasarAppUtils::Params::log("Log available in: " + QuasarAppUtils::Params::getArg("fileLog"));

}

CheatCardService::~CheatCardService() {
    if (_db) {
        _db->softDelete();
    }

    if (_server) {
        _server->softDelete();
    }
}

bool CheatCardService::onStart() {
    if (!_db) {
        _db = new RC::DataBase();
        _db->initSqlDb();
    }

    if (!_server) {
        _server = new RC::Server(_db->db());
    }

    if (!_server->run({}, DEFAULT_CHEAT_CARD_PORT)) {
        QuasarAppUtils::Params::log("Failed to start server!");
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
        result["1. Status"] = _server->getWorkState().toString();
        result["2. Log file available"] = QuasarAppUtils::Params::getArg("fileLog", "Not used");
        result["3. Core lib version"] = _server->libVersion();
        result["4. Heart lib version"] = QH::heartLibVersion();
        result["5. Patronum lib version"] = Patronum::patronumLibVersion();

        sendResuylt(result);
    } else if (data.cmd() == "setVerbose") {
        QuasarAppUtils::Params::setArg("verbose", data.arg());

        sendResuylt("New verbose level is " + QuasarAppUtils::Params::getArg("verbose"));
    } else if (data.cmd() == "clearData") {
        auto task = QSharedPointer<RC::ClearOldData>::create();
        task->setMode(QH::ScheduleMode::SingleWork);
        task->setTime(0);

        _server->sheduleTask(task);
        sendResuylt("Task are pushed");

    } else if (data.cmd() == "forceClearData") {
        auto task = QSharedPointer<RC::ClearOldData>::create(0);
        task->setMode(QH::ScheduleMode::SingleWork);
        task->setTime(0);

        _server->sheduleTask(task);
        sendResuylt("Task are pushed");
    }

    return true;
}

QSet<Patronum::Feature> CheatCardService::supportedFeatures() {
    QSet<Patronum::Feature> data;

    data << Patronum::Feature("ping", {}, "This is description of the ping command");
    data << Patronum::Feature("state", {}, "return state");
    data << Patronum::Feature("setVerbose", "verbose level", "sets new verbose log level");
    data << Patronum::Feature("clearData", {}, "Clear all old data from server");
    data << Patronum::Feature("forceClearData", {}, "clear all data from server");

    return data;
}

void CheatCardService::onResume() {
    onStart();
}

void CheatCardService::onPause() {
    _server->stop();
    sendResuylt("Server stopped successful. (paused)");
}
