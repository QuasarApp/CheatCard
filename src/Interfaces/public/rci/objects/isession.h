//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ISESSION_H
#define ISESSION_H

#include <QSharedPointer>
#include "rci/global.h"

namespace RC {
namespace Interfaces {

/**
 * @brief The iSession class this is rudement class that used only for apiv 1.5 and should be removed after drop support of this api.
 * @warning This is deprecated class ...
 */
class CHEATCARD_INTERFACES_EXPORT iSession
{

public:
    iSession() = default;
    virtual ~iSession() = default;


    /**
     * @brief isValid This method should be return true if this object is correct.
     * @return true if this object is correct else false.
     */
    virtual bool isValid() const = 0;

    virtual long long getSessionId() const = 0;
    virtual void setSessionId(long long newSessionId) = 0;

    virtual long long getUsercardId() const = 0;
    virtual void setUsercardId(long long newUsercardId) = 0;

};

}
}
Q_DECLARE_METATYPE(QSharedPointer<RC::Interfaces::iSession>)

#endif // ISESSION_H
