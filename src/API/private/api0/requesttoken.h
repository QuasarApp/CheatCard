//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef REQUESTTOKEN_H
#define REQUESTTOKEN_H


#include <streambase.h>

namespace RC {
namespace API {
namespace V0 {

class RequestToken: public QH::StreamBase {
public:
    RequestToken();
    unsigned long long requestToken() const;
    void setRequestToken(unsigned long long newRequestToken);
    unsigned long long responceToken() const;
    void setResponceToken(unsigned long long newResponceToken);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
private:

    unsigned long long _requestToken = 0;
    unsigned long long _responceToken = 0;

};
}
}
}
#endif // REQUESTTOKEN_H
