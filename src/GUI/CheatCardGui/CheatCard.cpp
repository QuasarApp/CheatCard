//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "CheatCard.h"

#include <CheatCard/database.h>
#include <SBarcodeFilter.h>
#include <SBarcodeGenerator.h>
#include <imageprovider.h>
#include "mainmodel.h"
#include "quasarapp.h"

namespace RC {

CheatCard::CheatCard() {
    _db = new DataBase();

}

CheatCard::~CheatCard() {
    delete _model;

    _db->softDelete();
}

bool CheatCard::init(QQmlApplicationEngine *engine) {
    initCheatCardResources();

    if (!QH::init()) {
        return false;
    }

    if (!_db->initSqlDb()) {
        QuasarAppUtils::Params::log("Failed to load database", QuasarAppUtils::Error);
    }

    engine->addImageProvider(QLatin1String("cards"), new ImageProvider(_db));

    auto root = engine->rootContext();

    _model = new MainModel(_db->db());

    root->setContextProperty("mainModel", QVariant::fromValue(_model));

    qmlRegisterType<SBarcodeGenerator>("com.scythestudio.scodes", 1, 0, "SBarcodeGenerator");
    qmlRegisterType<SBarcodeFilter>("com.scythestudio.scodes", 1, 0, "SBarcodeFilter");

    engine->load("qrc:/CheatCardModule/CheatCard.qml");
    if (engine->rootObjects().isEmpty())
        return false;

    return true;
}


}
