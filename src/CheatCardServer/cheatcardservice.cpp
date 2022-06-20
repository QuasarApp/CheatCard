//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardservice.h"
#include <QDateTime>
#include <CheatCard/serverssl.h>
#include <CheatCard/api/apiv1.h>

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

    if (_serverSSL) {
        _serverSSL->softDelete();
    }
}

bool CheatCardService::onStart() {
    if (!_db) {
        _db = new RC::DataBase();
        _db->initSqlDb();
    }

    if (!_serverSSL) {
        _serverSSL = new RC::ServerSSL(_db->db());
        _serverSSL->addApiParser<RC::ApiV1>();
    }

    if (!_serverSSL->run({}, DEFAULT_CHEAT_CARD_PORT_SSL)) {
        QuasarAppUtils::Params::log("Failed to start ssl server!");
        return false;
    }

    return true;
}

bool CheatCardService::handleReceive(const Patronum::Feature &data) {

    if (!_serverSSL) {
        sendResuylt("Service is running but server is offline. Send start comand");
        return true;
    }

    if (data.cmd() == "ping") {
        sendResuylt("Pong");
    } else if (data.cmd() == "state") {
        QVariantMap result;
        result["00. Server Status:"] = _serverSSL->getWorkState().toString();

        result["01. Status"] = _serverSSL->getWorkState().toString();
        result["02. Log file available"] = QuasarAppUtils::Params::getArg("fileLog", "Not used");
        result["03. Core lib version"] = _serverSSL->libVersion();
        result["04. Heart lib version"] = QH::heartLibVersion();
        result["05. Patronum lib version"] = Patronum::patronumLibVersion();

        sendResuylt(result);
    } else if (data.cmd() == "setVerbose") {
        QuasarAppUtils::Params::setArg("verbose", data.arg());

        sendResuylt("New verbose level is " + QuasarAppUtils::Params::getArg("verbose"));
    }

    return true;
}

QSet<Patronum::Feature> CheatCardService::supportedFeatures() {
    QSet<Patronum::Feature> data;

    data << Patronum::Feature("ping", {}, "This is description of the ping command");
    data << Patronum::Feature("state", {}, "return state");
    data << Patronum::Feature("setVerbose", "verbose level", "sets new verbose log level");

    return data;
}

void CheatCardService::onResume() {
    onStart();
}

void CheatCardService::onPause() {
    _serverSSL->stop();

    sendResuylt("Server stopped successful. (paused)");
}
