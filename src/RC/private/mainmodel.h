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

namespace RC {

class Card;
class CardModel;
class User;
class Config;
class CardsListModel;
class UserModel;
class ItemsModel;
class WaitConnectionModel;
class BaseNode;
class UsersCards;

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fFirst READ fFirst  NOTIFY fFirstChanged)

    Q_PROPERTY(QObject * currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QObject * cardsList READ cardsList NOTIFY cardsListChanged)
    Q_PROPERTY(int mode READ getMode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(QObject * defaultLogosModel READ defaultLogosModel NOTIFY defaultLogosModelChanged)
    Q_PROPERTY(QObject * defaultBackgroundsModel READ defaultBackgroundsModel NOTIFY defaultBackgroundsModelChanged)
    Q_PROPERTY(QObject * waitModel READ waitModel NOTIFY waitModelChanged)



public:
    enum class Mode {
        Unknown = -1,
        Client = 0,
        Seller = 1
    };
    MainModel(QH::ISqlDBCache* db);
    ~MainModel();

    bool fFirst() const;
    Q_INVOKABLE void configureFinished();
    QObject *currentUser() const;
    void setCurrentUser(UserModel *newCurrentUser);
    void setCurrentUser(QSharedPointer<UserModel> newCurrentUser);

    QObject *cardsList() const;
    QObject *defaultLogosModel() const;
    QObject *defaultBackgroundsModel() const;

    int getMode() const;
    void setMode(int newMode);

    QObject *waitModel() const;
signals:

    void fFirstChanged();
    void currentUserChanged();

    void cardsListChanged();

    void defaultLogosModelChanged();

    void defaultBackgroundsModelChanged();

    void connectionWasBegin();
    void connectionWasEnd();
    void freeItem(QObject *cardId, unsigned int freeItemsCount);


    void modeChanged();

    void waitModelChanged();

private slots:
    void handleCardReceived(QSharedPointer<Card> card);

    void handleCardEditFinished(const QSharedPointer<RC::Card> &card);

    void handleCardRemoved(int id);
    void handleCardSelectedForWork(const QSharedPointer<CardModel>& card);
    void handleConnectWasBegin();
    void handleConnectWasFinished();

    void handlePurchaseWasSuccessful(QSharedPointer<UsersCards>);
    void handleListenStart(int purchasesCount, QSharedPointer<CardModel> model);
    void handleListenStop();


private:
    void saveConfig();
    void saveUser();

    QSharedPointer<UserModel> initUser();

    QSharedPointer<Config> initConfig(int userId);

    void initCardsListModels();
    void initImagesModels();
    void initBackEndModel();
    void initWaitConnectionModel();

    void setCardListModel(CardsListModel *model);

    void initMode(const QSharedPointer<UserModel>& user,
                  const QSharedPointer<Config>& config);

    QH::ISqlDBCache * _db = nullptr;
    QSharedPointer<UserModel> _currentUser;
    QSharedPointer<Config> _config;

    CardsListModel *_currentCardsListModel = nullptr;
    CardsListModel *_cardsListModel = nullptr;
    CardsListModel *_ownCardsListModel = nullptr;

    ItemsModel *_defaultLogosModel = nullptr;
    ItemsModel *_defaultBackgroundsModel = nullptr;
    BaseNode * _backEndModel = nullptr;
    WaitConnectionModel * _waitModel = nullptr;
    QSettings _settings;

    Mode _mode = Mode::Unknown;
};

}
#endif // RC_MAINMODEL_H
