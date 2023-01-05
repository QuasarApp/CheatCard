//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef WAITCONFIRMMODEL_H
#define WAITCONFIRMMODEL_H

#include "basemodel.h"

#include <QObject>
#include <QTimer>

namespace RC {

/**
 * @brief The WaitConfirmModel class This model show waiting server responce view
 */
class WaitConfirmModel: public QObject, public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(bool waitConfirm READ waitConfirm NOTIFY waitConfirmChanged)

public:
    WaitConfirmModel();
    bool wait(long long waitFor);
    void confirm(long long confirmedId, bool success);
    bool waitConfirm() const;

signals:
    void waitConfirmChanged();

private slots:
    void handleTimeOut();

private:
    void disbaleWaiting();
    void setWaitConfirm(bool wait);

    long long _notConfirmedSessions = 0;
    bool _waitConfirm = false;
    QTimer _timer;

};
}
#endif // WAITCONFIRMMODEL_H
