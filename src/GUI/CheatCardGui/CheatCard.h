//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CheatCard_H
#define CheatCard_H

#include "gui_global.h"
#include "ibilling.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

inline void initCheatCardResources() {
    Q_INIT_RESOURCE(CheatCardGui);
    Q_INIT_RESOURCE(CardsIcons);
}

namespace RC {

class MainModel;

namespace Interfaces {
class iDB;
}

/**
 * @brief The CheatCard class
 */
class CHEATCARD_GUI_EXPORT CheatCard {

public:
    CheatCard();
    ~CheatCard();

    /**
     * @brief init This method initialize the main application class.
     * @brief billingObject This is billing object of application.
     * @return true if initialization finished successful
     */
    bool init(QQmlApplicationEngine *engine, RC::IBilling* billingObject);

private:
    /**
     * @brief initLang initialize languages
     */
    void initLang();

    QSharedPointer<Interfaces::iDB> _db;
    MainModel* _model = nullptr;
};
};

#endif //CheatCard_H
