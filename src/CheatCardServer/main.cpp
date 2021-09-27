//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QCoreApplication>
#include <quasarapp.h>

int main(int argc, char *argv[]) {

    QuasarAppUtils::Params::parseParams(argc, argv);

    QCoreApplication::setOrganizationName("QuasarApp");
    QCoreApplication::setApplicationName("CheatCardServer");

    QCoreApplication app(argc, argv);

    return app.exec();
}
