//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "serverssl.h"

#include <QCoreApplication>

namespace RC {

ServerSSL::ServerSSL(QH::ISqlDBCache *db): Server(db) {
    QH::SslSrtData sslData;
    sslData.commonName = getServerHost();
    sslData.organization = QCoreApplication::organizationName();

    useSelfSignedSslConfiguration(sslData);
}

}
