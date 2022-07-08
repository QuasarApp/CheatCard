//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IMPORTEXPORTUSERMODEL_H
#define IMPORTEXPORTUSERMODEL_H

#include <QObject>
#include <SBarcodeDecoder.h>

namespace RC {
class ImportExportUserModel: public QObject {
    Q_OBJECT
public:
    ImportExportUserModel();
    Q_INVOKABLE bool processQrCode(QString path);

public slots:
    void handleDecodeFinished(const QString &data);

signals:
    void decodeFinished(QString data);

private:
    SBarcodeDecoder _decoder;
};
}


#endif // IMPORTEXPORTUSERMODEL_H
