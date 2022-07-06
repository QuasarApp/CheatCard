
//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef GETCONNTACTSREQUEST_H
#define GETCONNTACTSREQUEST_H
#include <abstractdata.h>

namespace RC {
namespace APIv1_5 {

/**
 * @brief The GetConntactsRequest class
 */
class GetConntactsRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(APIv1_5::GetConntactsRequest)

public:
    GetConntactsRequest();
    bool isValid() const override;


    const QByteArray &getUserKey() const;
    void setUserKey(const QByteArray &newUserKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray userKey = 0;
};

}
}
#endif // GETCONNTACTSREQUEST_H
