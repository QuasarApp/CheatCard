//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef VERSIONFOROLDAPP_H
#define VERSIONFOROLDAPP_H

#include <abstractdata.h>
namespace RC {
namespace API {
namespace V3 {
/**
 * @brief The VersionForOldApp class send version of api for the old version of the app.
 */
class VersionForOldApp: public QH::PKG::AbstractData
{
public:
    VersionForOldApp();
    static unsigned short command();
    static QString commandText();
    unsigned short cmd() const override;
    QString cmdString() const override;
    bool isValid() const override;

protected:
    unsigned int localCode() const override;

    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

};

}
}
}

#endif // VERSIONFOROLDAPP_H
