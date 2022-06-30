//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef WAITCONNECTIONMODEL_H
#define WAITCONNECTIONMODEL_H


#include <QObject>
#include <QTimer>
#include "cardmodel.h"

class QTimer;

namespace RC {

namespace API {
class UserHeader;
class Session;
}

class WaitConnectionModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* card READ card NOTIFY cardChanged)
    Q_PROPERTY(int purchaseCount READ purchaseCount WRITE setPurchaseCount NOTIFY purchaseCountChanged)
    Q_PROPERTY(QString extraData READ extraData WRITE setExtraData NOTIFY extraDataChanged)
    Q_PROPERTY(bool allowScreenDim READ allowScreenDim WRITE setAllowScreenDim NOTIFY allowScreenDimChanged)
    Q_PROPERTY(bool waitConfirm READ waitConfirm NOTIFY waitConfirmChanged)

public:
    WaitConnectionModel();

    QObject *card() const;
    void setCard(const QSharedPointer<CardModel> &newCard);

    int purchaseCount() const;
    void setPurchaseCount(int newPurchaseCount);

    Q_INVOKABLE void begin();
    Q_INVOKABLE void cancel();

    const QString &extraData() const;
    void setExtraData(const QString &newExtraData);

    bool allowScreenDim() const;
    void setAllowScreenDim(bool newAllowScreenDim);

    bool waitConfirm() const;
    void setWaitConfirm(bool wait);

public slots:
    void handleSessionServerResult(QSharedPointer<RC::API::Session> session, bool succesed);

signals:
    void cardChanged();
    void purchaseCountChanged();
    void timeOutChanged();
    void purchaseTaskCompleted(int purchasesCount,
                               QSharedPointer<RC::CardModel> card,
                               QSharedPointer<RC::API::UserHeader>);

    void extraDataChanged();

    void cameraIdChanged();

    void allowScreenDimChanged();

    void waitConfirmChanged();

private slots:
    void handleTimeOut();

private:
    void disbaleWaiting();

    long long _notConfirmedSessions = 0;
    bool _waitConfirm = false;
    QTimer _timer;

    QSharedPointer<CardModel> _card = nullptr;
    int _purchaseCount = 1;

    QString _extraData;
    bool _allowScreenDim = true;
};
}
#endif // WAITCONNECTIONMODEL_H
