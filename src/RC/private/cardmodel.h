//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QObject>
#include <qqml.h>

namespace RC {

class Card;

/**
 * @brief The CardModel class
 */
class CardModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY objChanged)
    Q_PROPERTY(QString name READ name NOTIFY objChanged)
    Q_PROPERTY(int purchasesNumber READ purchasesNumber NOTIFY objChanged)
    Q_PROPERTY(int freeIndex READ freeIndex NOTIFY objChanged)

    QML_ELEMENT
public:
    CardModel();

    int id() const;
    QString name() const;
    int purchasesNumber() const;
    int freeIndex() const;

    Card *card() const;
    void setCard(Card *newCard);

signals:
    void objChanged();

private:
    Card *_card = nullptr;
};

}
#endif // CARDMODEL_H
