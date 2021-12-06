//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef IPLATFORMTOOLS_H
#define IPLATFORMTOOLS_H
#include <QtGlobal>

namespace RC {


/**
 * @brief The IPlatformTools class
 */
class IPlatformTools
{
public:
    virtual ~IPlatformTools() = default;

    static IPlatformTools* instance();

    /**
     * @brief setScreanDim disable or enable screan dim
     * @param enable
     */
    virtual void setScreanDim(bool enable) const = 0;

protected:
    IPlatformTools() = default;

};
}
#endif // IPLATFORMTOOLS_H
