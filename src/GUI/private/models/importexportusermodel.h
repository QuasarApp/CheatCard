//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IMPORTEXPORTUSERMODEL_H
#define IMPORTEXPORTUSERMODEL_H

#include "basemodel.h"
#include "qrcodedecoder.h"

namespace RC {
class ImportExportUserModel: public QRCodeDecoder, public BaseModel {
    Q_OBJECT
public:
    ImportExportUserModel();
};
}


#endif // IMPORTEXPORTUSERMODEL_H
