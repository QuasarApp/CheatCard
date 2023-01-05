//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef API_H
#define API_H

#include "api/apibase.h"

namespace RC {

/**
 * @brief API namespace is base name space for all API objects of the cheatcard app.
 */
namespace API {

/**
 * @brief init This method initialize the local api objects to the @a mainNode.
 * @param neededVersions This is list of needed api objects.
 * @param dataBase This is shared pointer of the main database connection.
 * @param mainNode this is raw pointer to the raw node object
 * @return hash map of the supported api objects.
 * @note If you invoke this method multiple times the it will create dublicate of pai objects.
 */
QHash<unsigned short, QSharedPointer<APIBase>>
    CHEATCARD_API_EXPORT init(const QVector<unsigned short> &neededVersions,
                              const QSharedPointer<Interfaces::iDB>& dataBase,
                              QH::AbstractNode* mainNode);

}
}

#endif // API_H
