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

class CHEATCARD_CORE_EXPORT RestoreDataRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE(RestoreDataRequest, "RestoreDataRequest")

public:
    RestoreDataRequest();
    unsigned int userId() const;
    void setUserId(unsigned int newUserId);
    bool isValid() const override;

    // StreamBase interface
protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _userId;

};
}
#endif // RESTOREDATAREQUEST_H
