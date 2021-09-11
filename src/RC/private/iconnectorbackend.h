//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ICONNECTORBACKEND_H
#define ICONNECTORBACKEND_H

#include <QByteArray>
#include <QSharedPointer>

namespace RC {

class Card;
class ITargetNode;

class IConnectorBackEnd
{
public:

    enum Mode {
        Client,
        Saller,
    };

    enum Commands {
        UserId,
        SallerId,
        CardId,
        CardData,
        UserData,
        PurchasesCount
    };

    IConnectorBackEnd();

    bool start(Mode mode);
    bool stop();

    QSharedPointer<Card> activeCard() const;
    void setActiveCard(QSharedPointer<Card> newActiveCard);

protected:

    virtual bool listen(Mode mode) = 0;

    virtual bool close() = 0;

    void connectionReceived( ITargetNode *obj);
    void connectionLost(ITargetNode* id);

protected slots:
    void handleReceiveMessage(const QByteArray& message);

private:

    Mode _mode = Client;
    QSharedPointer<ITargetNode> _currentTarget;
    QSharedPointer<Card> _activeCard;
};

}
#endif // ICONNECTORBACKEND_H
