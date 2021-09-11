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
#include "database.h"

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
    MainModel(DB* db);
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

private slots:
    void handleCardCreated(QSharedPointer<CardModel> card);
    void handleCardEditFinished(const QSharedPointer<RC::Card> &card);

    void handleCardRemoved(int id);

private:
    void saveConfig();
    void saveUser();

    QSharedPointer<UserModel> initUser();

    QSharedPointer<Config> initConfig(int userId);

    DB * _db = nullptr;
    QSharedPointer<UserModel> _currentUser;
    QSharedPointer<Config> _config;
    CardsListModel *_cardsListModel = nullptr;
    CardsListModel *_ownCardsListModel = nullptr;
    ItemsModel *_defaultLogosModel = nullptr;
    ItemsModel *_defaultBackgroundsModel = nullptr;

    QSettings _settings;

};

}
#endif // RC_MAINMODEL_H
