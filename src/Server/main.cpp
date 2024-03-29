//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardservice.h"

#include <QCoreApplication>
#include <quasarapp.h>

int main(int argc, char *argv[]) {

    QuasarAppUtils::Params::parseParams(argc, argv);

    QCoreApplication::setOrganizationName("QuasarApp");
    QCoreApplication::setApplicationName("CheatCardServer");

    CheatCardService app(argc, argv);

    return app.exec();
}
