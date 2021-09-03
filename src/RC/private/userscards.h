//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_H
#define USERSCARDS_H

#include <dbobject.h>

namespace RC {


class UsersCards: public QH::PKG::DBObject
{
public:
    UsersCards(int user, int card, bool owner = false);

public:
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    bool isValid() const override;
protected:
    QString primaryKey() const override;

private:
    int user = 0;
    int card = 0;
    bool owner = false;
};

}
#endif // USERSCARDS_H
