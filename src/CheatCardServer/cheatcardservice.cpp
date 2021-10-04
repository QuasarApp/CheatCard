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

void CheatCardService::onStart() {
    if (!_db) {
        _db = new RC::DataBase;
        _db->initSqlDb();
    }

    if (!_server) {
        _server = new RC::Server(_db->db());
    }

    if (!_server->run({}, DEFAULT_CHEAT_CARD_PORT)) {
        QuasarAppUtils::Params::log("Failed to start server!");
    }
}

void CheatCardService::onStop() {
    _server->stop();
    QCoreApplication::quit();
}

bool CheatCardService::handleReceive(const Patronum::Feature &data) {

    if (!_server) {
        sendResuylt("Service is running but server is offline. Send start comand");
        return true;
    }

    if (data.cmd() == "ping") {
        sendResuylt("Pong");
    } else if (data.cmd() == "state") {
        QString result = _server->getWorkState().toString();
        result += "\n Lo file available in - " + QuasarAppUtils::Params::getArg("fileLog", "not used");
        result += "\n core lib version: " + _server->libVersion();
        result += "\n heart lib version: " + QH::heartLibVersion();
        result += "\n patronum lib version: " + Patronum::patronumLibVersion();

        sendResuylt(result);
    }

    return true;
}

QSet<Patronum::Feature> CheatCardService::supportedFeatures() {
    QSet<Patronum::Feature> data;

    data << Patronum::Feature("ping", "This is description of the ping command");
    data << Patronum::Feature("state", "return state");

    return data;
}
