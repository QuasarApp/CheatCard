//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef STATUSAFTERCHANGES_H
#define STATUSAFTERCHANGES_H

#include "CheatCard/api/api0/session.h"

namespace RC {
namespace APIv1_5 {

/**
 * @brief The StatusAfterChanges class contains result of the changes.
 */
class StatusAfterChanges: public API::Session
{
    QH_PACKAGE_AUTO(APIv1_5::StatusAfterChanges)

public:
    StatusAfterChanges();
    ~StatusAfterChanges() override;

    bool status() const;
    void setStatus(bool newStatus);

    // StreamBase interface
protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    bool _status = false;

};
}
}
#endif // STATUSAFTERCHANGES_H
