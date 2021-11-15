#ifndef APPLICATIONVERSION_H
#define APPLICATIONVERSION_H

#include <abstractdata.h>


namespace RC {

class ApplicationVersion: public QH::PKG::AbstractData
{
    QH_PACKAGE(ApplicationVersion, "ApplicationVersion")

public:
    ApplicationVersion();

    // StreamBase interface
    int version() const;
    void setVersion(int newVersion);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    int _version = 0;

};

}

#endif // APPLICATIONVERSION_H
