//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef WAITCONNECTIONMODEL_H
#define WAITCONNECTIONMODEL_H


#include <QObject>
#include "cardmodel.h"

class QTimer;

namespace RC {

class WaitConnectionModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* card READ card NOTIFY cardChanged)
    Q_PROPERTY(int purchaseCount READ purchaseCount WRITE setPurchaseCount NOTIFY purchaseCountChanged)

    Q_PROPERTY(bool waitForConnect READ waitForConnect NOTIFY waitForConnectChanged)
    Q_PROPERTY(QString extraData READ extraData WRITE setExtraData NOTIFY extraDataChanged)

public:
    WaitConnectionModel();

    QObject *card() const;
    void setCard(const QSharedPointer<CardModel> &newCard);

    int purchaseCount() const;
    void setPurchaseCount(int newPurchaseCount);

    Q_INVOKABLE void begin();
    Q_INVOKABLE void cancel();

    bool waitForConnect() const;

    const QString &extraData() const;
    void setExtraData(const QString &newExtraData);

public slots:
    void handlePurchaseTaskFinished();

signals:
    void cardChanged();
    void purchaseCountChanged();
    void timeOutChanged();
    void purchaseTaskCompleted(int purchasesCount,
                               QSharedPointer<CardModel> card,
                               const QString& extraData);
    void purchaseTaskCanceled();
    void purchaseTaskFinished();

    void waitForConnectChanged();

    void extraDataChanged();

private:

    void setWaintForCnnect(bool val);

    QSharedPointer<CardModel> _card = nullptr;
    int _purchaseCount = 1;

    bool _waitForConnect = false;
    QString _extraData;
};
}
#endif // WAITCONNECTIONMODEL_H
