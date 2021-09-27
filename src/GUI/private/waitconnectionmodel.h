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
    Q_PROPERTY(int timeOut READ timeOut NOTIFY timeOutChanged)
    Q_PROPERTY(int waitTime READ waitTime NOTIFY waitTimeChanged)

    Q_PROPERTY(bool waitForConnect READ waitForConnect NOTIFY waitForConnectChanged)

public:
    WaitConnectionModel();

    QObject *card() const;
    void setCard(const QSharedPointer<CardModel> &newCard);

    int purchaseCount() const;
    void setPurchaseCount(int newPurchaseCount);

    int waitTime() const;

    int timeOut() const;
    void setTimeOut(int newTimeOut);

    Q_INVOKABLE void begin();
    Q_INVOKABLE void cancel();

    bool waitForConnect() const;

public slots:
    void handlePurchaseTaskFinished();

signals:
    void cardChanged();
    void purchaseCountChanged();
    void timeOutChanged();
    void purchaseTaskCompleted(int purchasesCount,
                               QSharedPointer<CardModel> card);
    void purchaseTaskCanceled();
    void purchaseTaskFinished();

    void waitTimeChanged();
    void waitForConnectChanged();

private slots:
    void handleTick();
private:

    void setWaintForCnnect(bool val);

    QSharedPointer<CardModel> _card = nullptr;
    int _purchaseCount = 1;
    int _timeOut = 10; //secundes

    QTimer *_timer = nullptr;
    bool _waitForConnect = false;
};
}
#endif // WAITCONNECTIONMODEL_H
