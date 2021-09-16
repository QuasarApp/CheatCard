//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "RegularCustomer_global.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

inline void initRegularCustomerResources() {
    Q_INIT_RESOURCE(RegularCustomer);
    Q_INIT_RESOURCE(CardsIcons);
}

namespace RC {

class DataBase;
class MainModel;

/**
 * @brief The RegularCustomer class
 */
class RegularCustomer_EXPORT RegularCustomer {

public:
    RegularCustomer();
    ~RegularCustomer();

    /**
     * @brief init This method initialize the main application class.
     * @return true if initialization finished successful
     */
    bool init(QQmlApplicationEngine *engine);

private:

    DataBase * _db = nullptr;
    MainModel* _model = nullptr;
};
};
