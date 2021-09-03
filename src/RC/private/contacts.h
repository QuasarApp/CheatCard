//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SALLERSUSERS_H
#define SALLERSUSERS_H
#include <dbobject.h>


namespace RC {

class Contacts: public QH::PKG::DBObject
{
public:
    Contacts(int user, int contactUser);
    // DBObject interface
public:
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
protected:
    QString primaryKey() const override;

private:
    int contactUser = 0;
    int user = 0;
};

}
#endif // SALLERSUSERS_H
