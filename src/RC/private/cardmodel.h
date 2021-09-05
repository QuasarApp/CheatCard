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
#include "card.h"

namespace RC {

/**
 * @brief The CardModel class
 */
class CardModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY objChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)
    Q_PROPERTY(int purchasesNumber READ purchasesNumber WRITE setPurchasesNumber NOTIFY objChanged)
    Q_PROPERTY(int freeIndex READ freeIndex WRITE setFreeIndex NOTIFY objChanged)

    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY objChanged)
    Q_PROPERTY(QString telegramm READ telegramm WRITE setTelegramm NOTIFY objChanged)
    Q_PROPERTY(QString instagramm READ instagramm WRITE setInstagramm NOTIFY objChanged)
    Q_PROPERTY(QString physicalAddress READ physicalAddress WRITE setPhysicalAddress NOTIFY objChanged)
    Q_PROPERTY(QString webSite READ webSite WRITE setWebSite NOTIFY objChanged)
    Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY objChanged)

    QML_ELEMENT
public:
    CardModel();
    CardModel(Card* card);

    int id() const;
    QString name() const;
    int purchasesNumber() const;
    int freeIndex() const;

    Card *card() const;
    void setCard(Card *newCard);

    QString phone() const;
    void setPhone(const QString &newPhone);

    QString telegramm() const;
    void setTelegramm(const QString &newTelegramm);

    QString instagramm() const;
    void setInstagramm(const QString &newInstagramm);

    QString physicalAddress() const;
    void setPhysicalAddress(const QString &newPhysicalAddress);

    QString webSite() const;
    void setWebSite(const QString &newWebSite);

    QString getColor() const;
    void setColor(const QString &newColor);

    void setName(const QString &newName);

    void setPurchasesNumber(int newPurchasesNumber);

    void setFreeIndex(int newFreeIndex);

    Q_INVOKABLE void save();
signals:
    void objChanged();
    void editFinished(Card* card);

private:
    Card *_card = nullptr;

};

}
#endif // CARDMODEL_H
