//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "RegularCustomer.h"

#include <database.h>
#include <imageprovider.h>
#include "mainmodel.h"
#include "quasarapp.h"
#include "mainmodel.h"

namespace RC {

RegularCustomer::RegularCustomer() {
    _db = new DataBase();

}

RegularCustomer::~RegularCustomer() {
    delete _model;

    _db->softDelete();
}

bool RegularCustomer::init(QQmlApplicationEngine *engine) {
    initRegularCustomerResources();

    if (!QH::init()) {
        return false;
    }

    if (!_db->initSqlDb()) {
        QuasarAppUtils::Params::log("Failed to load database", QuasarAppUtils::Error);
    }

    engine->addImageProvider(QLatin1String("userItems"), new ImageProvider(_db));

    auto root = engine->rootContext();

    _model = new MainModel(_db->db());

    root->setContextProperty("mainModel", QVariant::fromValue(_model));

    engine->load("qrc:/RegularCustomerModule/RegularCustomer.qml");
    if (engine->rootObjects().isEmpty())
        return false;

    return true;
}


}
