//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARDSERVICE_H
#define CHEATCARDSERVICE_H

#include <patronum.h>
#include <CheatCard/server.h>
#include <rci/core/idb.h>

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

    QVariantMap prepareStatistic(const QByteArray &card) const;
    QVariantMap cardList(const QByteArray& user = 0) const;

    RC::Server *_server = nullptr;
    QString dbBackPath;
    QSharedPointer<RC::Interfaces::iDB> _db;


};

#endif // CHEATCARDSERVICE_H
