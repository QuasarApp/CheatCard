//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef SALLERSCARDS_H
#define SALLERSCARDS_H

#include <dbobject.h>

namespace RC {

class SallersCards: public QH::PKG::DBObject
{
public:
    SallersCards(int saller, int card );

    // DBObject interface
public:
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;
    bool fromSqlRecord(const QSqlRecord &q) override;

protected:
    QString primaryKey() const override;

private:
    int saller = 0;
    int card = 0;
};
}
#endif // SALLERSCARDS_H
