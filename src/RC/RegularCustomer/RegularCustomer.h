//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "RegularCustomer_global.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

inline void initRegularCustomerResources() { Q_INIT_RESOURCE(RegularCustomer); }

namespace RC {

/**
 * @brief The RegularCustomer class
 */
class RegularCustomer_EXPORT RegularCustomer {

public:
    RegularCustomer();

    /**
     * @brief init This method initialize the main application class.
     * @return true if initialization finished successful
     */
    bool init(QQmlApplicationEngine *engine);
};
};
