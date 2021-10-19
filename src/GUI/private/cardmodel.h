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
#include "CheatCard/card.h"

namespace RC {

/**
 * @brief The CardModel class
 */
class CardModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int id READ id NOTIFY objChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY objChanged)

    Q_PROPERTY(int purchasesNumber READ purchasesNumber NOTIFY purchasesNumberChanged)
    Q_PROPERTY(int freeIndex READ freeIndex WRITE setFreeIndex NOTIFY objChanged)

    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY objChanged)
    Q_PROPERTY(QString telegramm READ telegramm WRITE setTelegramm NOTIFY objChanged)
    Q_PROPERTY(QString instagramm READ instagramm WRITE setInstagramm NOTIFY objChanged)
    Q_PROPERTY(QString physicalAddress READ physicalAddress WRITE setPhysicalAddress NOTIFY objChanged)
    Q_PROPERTY(QString webSite READ webSite WRITE setWebSite NOTIFY objChanged)
    Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY objChanged)
    Q_PROPERTY(QString fontColor READ getFontColor WRITE setFontColor NOTIFY objChanged)
    Q_PROPERTY(QString freeItem READ getFreeItem WRITE setFreeItem NOTIFY objChanged)

    QML_ELEMENT
public:
    CardModel();
    CardModel(QSharedPointer<Card> card);

    unsigned int id() const;
    int purchasesNumber() const;
    int freeIndex() const;

    QSharedPointer<Card> card() const;
    void setCard(const QSharedPointer<Card> &newCard);

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

    QString getFontColor() const;
    void setFontColor(const QString &newColor);

    void setPurchasesNumber(int newPurchasesNumber);

    void setFreeIndex(int newFreeIndex);

    Q_INVOKABLE void save();

    Q_INVOKABLE void setNewBackGround(const QString &backgroundPath);
    Q_INVOKABLE void setNewLogo(const QString &logoPath);
    Q_INVOKABLE void setNewSeel(const QString &seelPath);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString getFreeItem() const;
    void setFreeItem(const QString &newFreeItem);

signals:
    void objChanged();
    void editFinished(const QSharedPointer<Card>& card);
    void purchasesNumberChanged();

private:

    QByteArray convert(const QString& imagePath);

    QSharedPointer<Card> _card = nullptr;
    int _purchasesNumber = 1;
};

}
#endif // CARDMODEL_H
