//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APPLICATIONVERSION_H
#define APPLICATIONVERSION_H

#include <abstractdata.h>


namespace RC {

class ApplicationVersion: public QH::PKG::AbstractData
{
    QH_PACKAGE(ApplicationVersion, "ApplicationVersion")

public:
    ApplicationVersion();

    short minimum() const;
    void setMinimum(short newMinimum);

    short maximum() const;
    void setMaximum(short newMaximum);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    short _minimum = 0;
    short _maximum = 0;

};

}

#endif // APPLICATIONVERSION_H
