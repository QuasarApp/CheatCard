//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CREATECARDMODEL_H
#define CREATECARDMODEL_H

#include <QObject>
#include <cardmodel.h>

namespace RC {

class CreateCardModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject * currentCard READ currentCard NOTIFY currentCardChanged)

public:
    CreateCardModel();
    ~CreateCardModel();
    QObject *currentCard() const;

signals:
    void sigCardCreated(const QSharedPointer<API::Card>& );
    void currentCardChanged();

private slots:
    void handleCardEditFinished(const QSharedPointer<API::Card> &card);

private:
    void setCurrentCard(const QSharedPointer<API::Card>&);

    CardModel* _currentCard = nullptr;
};
}
#endif // CREATECARDMODEL_H
