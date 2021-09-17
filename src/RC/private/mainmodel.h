//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RC_MAINMODEL_H
#define RC_MAINMODEL_H

#include <QObject>
#include <QSettings>
#include <CheatCard/database.h>
#include "CheatCard/iconnectorbackend.h"

namespace RC {

class Card;
class CardModel;
class User;
class Config;
class CardsListModel;
class UserModel;
class ItemsModel;


/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fFirst READ fFirst  NOTIFY fFirstChanged)
    Q_PROPERTY(QObject * currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QObject * cardsList READ cardsList NOTIFY cardsListChanged)
    Q_PROPERTY(QObject * ownCardsList READ ownCardsList NOTIFY ownCardsListChanged)

    Q_PROPERTY(QObject * defaultLogosModel READ defaultLogosModel NOTIFY defaultLogosModelChanged)
    Q_PROPERTY(QObject * defaultBackgroundsModel READ defaultBackgroundsModel NOTIFY defaultBackgroundsModelChanged)


public:
    MainModel(QH::ISqlDBCache* db);
    ~MainModel();

    bool fFirst() const;
    Q_INVOKABLE void configureFinished();
    QObject *currentUser() const;
    void setCurrentUser(UserModel *newCurrentUser);
    void setCurrentUser(QSharedPointer<UserModel> newCurrentUser);

    QObject *cardsList() const;
    QObject *ownCardsList() const;
    QObject *defaultLogosModel() const;
    QObject *defaultBackgroundsModel() const;

signals:

    void fFirstChanged();
    void currentUserChanged();

    void cardsListChanged();
    void ownCardsListChanged();

    void defaultLogosModelChanged();

    void defaultBackgroundsModelChanged();

    void connectionWasBegin();
    void connectionWasEnd();
    void purchaseWasSuccessful(unsigned int cardId, unsigned int purcaseCount);


private slots:
    void handleCardCreated(QSharedPointer<CardModel> card);
    void handleCardReceived(QSharedPointer<Card> card);

    void handleCardEditFinished(const QSharedPointer<RC::Card> &card);

    void handleCardRemoved(int id);

    void handleConnectWasBegin();
    void handleConnectWasFinished();

    void handlePurchaseWasSuccessful(QSharedPointer<UsersCards>);


private:
    void saveConfig();
    void saveUser();

    QSharedPointer<UserModel> initUser();

    QSharedPointer<Config> initConfig(int userId);

    QH::ISqlDBCache * _db = nullptr;
    QSharedPointer<UserModel> _currentUser;
    QSharedPointer<Config> _config;
    CardsListModel *_cardsListModel = nullptr;
    CardsListModel *_ownCardsListModel = nullptr;
    ItemsModel *_defaultLogosModel = nullptr;
    ItemsModel *_defaultBackgroundsModel = nullptr;
    IConnectorBackEnd * _backEndModel = nullptr;

    QSettings _settings;

};

}
#endif // RC_MAINMODEL_H
