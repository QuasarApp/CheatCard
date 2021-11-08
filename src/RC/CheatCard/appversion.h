#ifndef APPVERSION_H
#define APPVERSION_H

#include <streambase.h>

namespace RC {

class AppVersion: protected QH::StreamBase
{
public:
    AppVersion() = default;
    ~AppVersion() = default;

    // StreamBase interface
    unsigned short version() const;
    void setVersion(unsigned short newVersion);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned short _version = 0;
};
}
#endif // APPVERSION_H
