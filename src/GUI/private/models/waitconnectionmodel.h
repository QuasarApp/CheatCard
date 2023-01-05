//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef WAITCONNECTIONMODEL_H
#define WAITCONNECTIONMODEL_H


#include <QObject>
#include <QTimer>
#include "cardmodel.h"

namespace RC {

namespace Interfaces {
class iSession;
}
class UserHeader;

class WaitConfirmModel;

class WaitConnectionModel: public QObject, public BaseModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* card READ card NOTIFY cardChanged)
    Q_PROPERTY(int purchaseCount READ purchaseCount WRITE setPurchaseCount NOTIFY purchaseCountChanged)
    Q_PROPERTY(QString extraData READ extraData WRITE setExtraData NOTIFY extraDataChanged)
    Q_PROPERTY(bool allowScreenDim READ allowScreenDim WRITE setAllowScreenDim NOTIFY allowScreenDimChanged)
    Q_PROPERTY(QObject* waitModel READ waitModel CONSTANT)

public:
    WaitConnectionModel();
    ~WaitConnectionModel() override;
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

    QObject *waitModel() const;

public slots:
    void handleSessionServerResult(QSharedPointer<RC::Interfaces::iSession> session, bool succesed);

signals:
    void cardChanged();
    void purchaseCountChanged();
    void timeOutChanged();
    void purchaseTaskCompleted(int purchasesCount,
                               QSharedPointer<RC::CardModel> card,
                               QSharedPointer<RC::UserHeader>);

    void extraDataChanged();

    void cameraIdChanged();

    void allowScreenDimChanged();

private:

    QSharedPointer<CardModel> _card = nullptr;
    int _purchaseCount = 1;

    QString _extraData;
    bool _allowScreenDim = true;
    WaitConfirmModel *_waitModel = nullptr;
};
}
#endif // WAITCONNECTIONMODEL_H
