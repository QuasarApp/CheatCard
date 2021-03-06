//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef UPDATECONTACTDATARESPONCE_H
#define UPDATECONTACTDATARESPONCE_H

#include <abstractdata.h>

namespace RC {
namespace APIv1_5 {

/**
 * @brief The UpdateContactDataResponce class
 */
class UpdateContactDataResponce: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(APIv1_5::UpdateContactDataResponce)

public:
    UpdateContactDataResponce();

    // AbstractData interface
    bool isValid() const override;

    // StreamBase interface
    bool getSuccessful() const;
    void setSuccessful(bool newSuccessful);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    bool successful = false;
};
}
}
#endif // UPDATECONTACTDATARESPONCE_H
