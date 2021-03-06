#ifndef OSXPLATFORMTOOLS_H
#define OSXPLATFORMTOOLS_H
#include <iplatformtools.h>

namespace RC {

class OSXPlatformTools: public IPlatformTools
{

public:
    void setScreanDim(bool enable) const override;
    bool getAccessToWriteInStorage() override;
    bool getAccessToReadInStorage() override;
    QString storeLink() const override;

protected:
    OSXPlatformTools();

    // IPlatformTools interface

    friend class IPlatformTools;
;

};
}
#endif // OSXPLATFORMTOOLS_H
