//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardservice.h"

CheatCardService::CheatCardService(int argc, char **argv):
    Patronum::Service<QCoreApplication>(argc, argv) {

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
}

bool CheatCardService::handleReceive(const Patronum::Feature &data) {

    if (!_server) {
        sendResuylt("Service is running but server is offline. Send start comand");
        return true;
    }

    if (data.cmd() == "ping") {
        sendResuylt("Pong");
    } else if (data.cmd() == "state") {

        sendResuylt(_server->getWorkState().toString());
    }

    return true;
}

QSet<Patronum::Feature> CheatCardService::supportedFeatures() {
    QSet<Patronum::Feature> data;

    data << Patronum::Feature("ping", "This is description of the ping command");
    data << Patronum::Feature("state", "return state");

    return data;
}
