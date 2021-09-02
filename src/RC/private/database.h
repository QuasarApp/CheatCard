//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DATABASE_H
#define DATABASE_H

#include "heart.h"

namespace RC {
/**
 * @brief The DataBase class This is main databae object.
 */
class DataBase: public QH::DataBaseNode
{
    Q_OBJECT
public:
    DataBase();
};

}
#endif // DATABASE_H
