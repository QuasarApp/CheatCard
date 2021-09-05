//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <dbobject.h>


#ifndef RC_CONFIG_H
#define RC_CONFIG_H


namespace RC {

class Config: public QH::PKG::DBObject
{
public:
    Config();
    Config(int userId, bool firstRun = true);

    // DBObject interface
public:
    QH::PKG::DBObject *createDBObject() const override;
    bool fromSqlRecord(const QSqlRecord &q) override;
    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;
    int getUserId() const;
    void setUserId(int newUserId);

    bool getFirstRun() const;
    void setFirstRun(bool newFirstRun);

protected:
    QString primaryKey() const override;

private:
    int userId = -1;
    bool firstRun = true;

};

}
#endif // RC_CONFIG_H
