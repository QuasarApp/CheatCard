//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "RegularCustomer.h"


namespace RC {

RegularCustomer::RegularCustomer() {

}

bool RegularCustomer::init(QQmlApplicationEngine *engine) {
    initRegularCustomerResources();

    engine->load("qrc:/RegularCustomerModule/RegularCustomer.qml");
    if (engine->rootObjects().isEmpty())
        return false;

    return true;
}


}
