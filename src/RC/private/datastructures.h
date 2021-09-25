//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QByteArray>
#include <abstractdata.h>


#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H


namespace RC {

class CardStatusRequest: public QH::PKG::AbstractData
{
public:
    CardStatusRequest();
    bool isValid() const override;
    // StreamBase interface

    unsigned long long getSessionId() const;
    void setSessionId(unsigned long long newSessionId);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned long long sessionId;
};

class UserHeader: public QH::PKG::AbstractData
{
public:
    UserHeader();
    bool isValid() const override;

    unsigned int getUserId() const;
    void setUserId(unsigned int newUserId);

    unsigned long long getSessionId() const;
    void setSessionId(unsigned long long newSessionId);

    const QByteArray &token() const;
    void setToken(const QByteArray &newToken);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int userId = 0;
    unsigned long long sessionId;
    QByteArray _token;
};


class CardDataRequest: public QH::PKG::AbstractData{

    // AbstractData interface
public:
    CardDataRequest();
    bool isValid() const override;
    // StreamBase interface

    unsigned int getCardId() const;
    void setCardId(unsigned int newCardId);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int cardId = 0;

};

class Successful: public QH::PKG::AbstractData{};

}

#endif // DATASTRUCTURES_H
