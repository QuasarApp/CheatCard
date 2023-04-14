//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef IDATACONVERTOR_H
#define IDATACONVERTOR_H
#include <QSharedPointer>

#include <rci/core/idb.h>

namespace RC {
namespace API {
/**
 * @brief The iDataConvertor class is base interface for the converting api objects into database objects.
 * @tparam ObjectInterfaceType it is name of tinterfeca.
 */
template<class ObjectInterfaceType>
class iDataConvertor
{
public:
    /**
     * @brief toObject this method should be convert api object intot database object.
     * @param db This is database. It's needet for create a new object of db.
     * @return interface of the database object.
     */
    virtual QSharedPointer<ObjectInterfaceType>
    toObject(const QSharedPointer<RC::Interfaces::iDB>& db) const = 0;

    /**
     * @brief fromObject This method copy data form the database object into this api object.
     * @param obj This is database object.
     */
    virtual void fromObject(const QSharedPointer<ObjectInterfaceType>& obj) = 0;
};

}
}

#endif // IDATACONVERTOR_H
