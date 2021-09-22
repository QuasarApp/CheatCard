//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SESSION_H
#define SESSION_H

#include "dbobject.h"

namespace RC {

class Session : public QH::PKG::DBObject
{
public:
    Session();

    // AbstractData interface
public:
    bool isValid() const override;
    QH::PKG::DBObject *createDBObject() const override;

    unsigned int getUser() const;
    void setUser(unsigned int newUser);

    unsigned int getCard() const;
    void setCard(unsigned int newCard);

    const QByteArray &getSessionId() const;
    void setSessionId(const QByteArray &newSessionId);

    bool fromSqlRecord(const QSqlRecord &q) override;
    QH::PKG::DBVariantMap variantMap() const override;
protected:
    QString primaryKey() const override;

private:
    unsigned int user = 0;
    unsigned int card = 0;
    QByteArray sessionId;
};

}
#endif // SESSION_H
