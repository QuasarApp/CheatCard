//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RESTOREDATAREQUEST_H
#define RESTOREDATAREQUEST_H

#include <abstractdata.h>
#include "CheatCard/core_global.h"

namespace RC {

namespace APIv1 {

class CHEATCARD_CORE_EXPORT RestoreDataRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE(RestoreDataRequest, "RestoreDataRequest")

public:
    RestoreDataRequest();
    bool isValid() const override;

    // StreamBase interface
    const QByteArray &userKey() const;
    QString userKeyBase64() const;
    void setUserKey(const QByteArray &newUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray _userKey;

};
}
}
#endif // RESTOREDATAREQUEST_H
