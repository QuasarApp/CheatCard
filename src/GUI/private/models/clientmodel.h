//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include "basemodel.h"

#include <CheatCard/client.h>


namespace RC {

class ClientModel: public Client, public BaseModel
{
public:
    ClientModel(const QSharedPointer<Interfaces::iDB> &db);
};
}
#endif // CLIENTMODEL_H
