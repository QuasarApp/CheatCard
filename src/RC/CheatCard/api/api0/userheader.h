//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef USERHEADER_H
#define USERHEADER_H

#include <abstractdata.h>
#include "CheatCard/core_global.h"

namespace RC {
namespace API {


class CHEATCARD_CORE_EXPORT UserHeader: public QH::PKG::AbstractData
{
    QH_PACKAGE(UserHeader, "UserHeader")

    public:
        UserHeader();
    bool isValid() const override;

    unsigned int getUserId() const;
    void setUserId(unsigned int newUserId);

    long long getSessionId() const;
    void setSessionId(long long newSessionId);

    const QByteArray &token() const;
    void setToken(const QByteArray &newToken);

    const QString &userName() const;
    void setUserName(const QString &newUserName);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _userId = 0;
    long long _sessionId;
    QByteArray _token;
    QString _userName;
};
}
}
#endif // USERHEADER_H
