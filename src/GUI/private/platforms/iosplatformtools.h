//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef IOSPLATFORMTOOLS_H
#define IOSPLATFORMTOOLS_H
#include <iplatformtools.h>

namespace RC {

class IOSPlatformTools: public IPlatformTools
{
public:
    void setScreanDim(bool enable) const override;
    bool getAccessToWriteInStorage() override;
    bool getAccessToReadInStorage() override;
    QString storeLink() const override;

protected:
    IOSPlatformTools();
    friend class IPlatformTools;

};
}
#endif // IOSPLATFORMTOOLS_H
