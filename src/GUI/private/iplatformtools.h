//#
//# Copyright (C) 2021-2023 QuasarApp.
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

    /**
     * @brief getAccessToWriteInStorage This method try allow access to external storage for write.
     */
    virtual bool getAccessToWriteInStorage() = 0;

    /**
     * @brief getAccessToWriteInStorage This method try allow access to external storage for read.
     */
    virtual bool getAccessToReadInStorage() = 0;

    /**
     * @brief storeLink This method return link to application in dependet of platfom application store.
     */
    virtual QString storeLink() const = 0;

protected:
    IPlatformTools() = default;

};
}
#endif // IPLATFORMTOOLS_H
