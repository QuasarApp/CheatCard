//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef USERHEADER_H
#define USERHEADER_H

#include <abstractdata.h>
#include <QSharedPointer>

namespace RC {

namespace Interfaces {
class iUser;
}

class UserHeader: public QH::PKG::AbstractData
{
    QH_PACKAGE(UserHeader, "UserHeader")
public:

    UserHeader();
    bool isValid() const override;

    const QString &userName() const;
    void setUserName(const QString &newUserName);

    bool toUser(QSharedPointer<Interfaces::iUser>& out) const;

    const QByteArray& userKey() const;
    void setUserKey(const QByteArray &newUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray _userKey;
    QString _userName;
};
}

Q_DECLARE_METATYPE(QSharedPointer<RC::UserHeader>)

#endif // USERHEADER_H
