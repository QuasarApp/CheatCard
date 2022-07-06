//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARDSERVICE_H
#define CHEATCARDSERVICE_H

#include <patronum.h>
#include <CheatCard/server.h>
#include <CheatCard/database.h>

/**
 * @brief The CheatCardService class This is main service wrapper class for cheat card
 */
class CheatCardService: public Patronum::Service<QCoreApplication>
{
public:
    CheatCardService(int argc, char **argv);
    ~CheatCardService() override;

    bool onStart() override;
    bool handleReceive(const Patronum::Feature &data) override;
    QSet<Patronum::Feature> supportedFeatures() override;
    void onResume() override;
    void onPause() override;
private:

    RC::Server *_serverSSL = nullptr;
    QString dbBackPath;
    RC::DataBase * _db = nullptr;


};

#endif // CHEATCARDSERVICE_H
