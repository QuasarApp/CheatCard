//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RC_MAINMODEL_H
#define RC_MAINMODEL_H

#include <QObject>
#include <QSettings>
#include <settingslistner.h>
#include <CheatCard/database.h>
#include <CheatCardGui/ibilling.h>

namespace RC {

namespace API {
class Card;
class User;
class UsersCards;
class UserHeader;

}

class SoundPlayback;
class CardModel;
class AboutModel;
class CardsListModel;
class UserModel;
class ItemsModel;
class WaitConnectionModel;
class BaseNode;
class IBilling;
class CardProxyModel;
class SellerStatisticModel;
class SettingsModel;
class ImportExportUserModel;

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject, public QuasarAppUtils::SettingsListner
{
    Q_OBJECT
    Q_PROPERTY(bool fFirst READ fFirst  NOTIFY fFirstChanged)

    Q_PROPERTY(QObject * currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QObject * cardsList READ cardsList NOTIFY cardsListChanged)
    Q_PROPERTY(int mode READ getMode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(QObject * defaultLogosModel READ defaultLogosModel NOTIFY defaultLogosModelChanged)
    Q_PROPERTY(QObject * defaultBackgroundsModel READ defaultBackgroundsModel NOTIFY defaultBackgroundsModelChanged)
    Q_PROPERTY(QObject * waitModel READ waitModel NOTIFY waitModelChanged)
    Q_PROPERTY(QObject * statisticModel READ statisticModel NOTIFY statisticModelChanged)
    Q_PROPERTY(QObject * exportImportModel READ exportImportModel NOTIFY exportImportModelChanged)


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
    Q_INVOKABLE QObject *getAboutModel();
    QObject *currentUser() const;

    const QSharedPointer<UserModel>& getCurrentUser() const;

    void setCurrentUser(UserModel *newCurrentUser);
    void setCurrentUser(QSharedPointer<UserModel> newCurrentUser);

    QObject *cardsList() const;
    QObject *defaultLogosModel() const;
    QObject *defaultBackgroundsModel() const;

    int getMode() const;
    void setMode(int newMode);

    QObject *waitModel() const;

    void initBilling(IBilling* billingObject);

    /**
     * @brief flush Save all temp users data and configuration
     */
    void flush();

    /**
     * @brief getReceivedItemsCount This method return count of all purches of @a cardId nad current user.
     * @param cardId This is card id.
     * @return count of of all purches of @a cardId nad current user. Else return 0
     */
    Q_INVOKABLE int getReceivedItemsCount(int cardId) const;

    QObject * statisticModel() const;

    QObject *exportImportModel() const;

public slots:
    void handleFirstDataSendet();
    void handleBonusGivOut(int userId, int cardId, int givOutcount);
    void handleNetworkError(QAbstractSocket::SocketError, QSslError::SslError sslErrorcode);

signals:

    void fFirstChanged();
    void currentUserChanged();

    void cardsListChanged();

    void defaultLogosModelChanged();

    void defaultBackgroundsModelChanged();

    void connectionWasBegin();
    void connectionWasEnd();
    void freeItem(QObject *cardId, int userId,
                  int freeItemsCount);


    void modeChanged();

    void waitModelChanged();
    void statisticModelChanged();

    void exportImportModelChanged();

    // SettingsListner interface
protected:
    void handleSettingsChanged(const QString &key, const QVariant &value) override;

private slots:
    bool handleImportUser(const QString &base64UserData);

    void handleCardReceived(QSharedPointer<RC::API::Card> card);

    void handleCardEditFinished(const QSharedPointer<RC::API::Card> &card);
    void handleResetCardModel(const QSharedPointer<RC::API::Card> &card);

    void handleRemoveRequest(const QSharedPointer<API::Card> &card);
    void handleCardSelectedForWork(const QSharedPointer<CardModel>& card);
    void handleCardSelectedForStatistic(const QSharedPointer<CardModel>& card);

    void handleConnectWasBegin();
    void handleConnectWasFinished();

    void handlePurchaseWasSuccessful(QSharedPointer<API::UsersCards>);
    void handleListenStart(int purchasesCount, QSharedPointer<CardModel> model, const QString &extraData);
    void handleListenStop();
    void handleAppStateChanged(Qt::ApplicationState state);
    void handlePurchaseReceived(Purchase purchase);
    void saveCard(const QSharedPointer<RC::API::Card> &card);

private:
    void saveConfig();
    void saveUser();
    QH::ISqlDBCache *db() const;

    QSharedPointer<UserModel> initUser();

    void initCardsListModels();
    void initImagesModels();
    void setBackEndModel(const QSharedPointer<BaseNode> &newModel);
    void initWaitConnectionModel();
    void initSellerStatisticModel();
    void initImportExportModel();

    void configureCardsList();

    void setCardListModel(CardsListModel *model);

    void initMode(const QSharedPointer<UserModel>& user);

    void soundEffectPlayback(const QString &soundName);

    bool sendSellerDataToServer(const QSharedPointer<API::UserHeader> &header,
                                unsigned int cardId,
                                int purchasesCount,
                                bool sendOnly);

    CardsListModel* getCurrentListModel() const;    

    QH::ISqlDBCache * _db = nullptr;
    QSharedPointer<UserModel> _currentUser;
    SettingsModel* _config = nullptr;

    CardsListModel *_cardsListModel = nullptr;
    CardsListModel *_ownCardsListModel = nullptr;

    CardProxyModel *_currentCardsListModel = nullptr;

    AboutModel *_aboutModel = nullptr;
    SoundPlayback *_soundEffect = nullptr;
    SellerStatisticModel *_statisticModel = nullptr;
    ItemsModel *_defaultLogosModel = nullptr;
    ItemsModel *_defaultBackgroundsModel = nullptr;

    ImportExportUserModel *_importExportModel = nullptr;
    IBilling *_billing = nullptr;

    QSharedPointer<BaseNode> _backEndModel = nullptr;
    QSharedPointer<BaseNode> _sellerbackEndModel = nullptr;
    QSharedPointer<BaseNode> _visitorbackEndModel = nullptr;

    WaitConnectionModel *_waitModel = nullptr;
    QSettings _settings;

    QSharedPointer<API::UserHeader> _lastUserHeader;

    Mode _mode = Mode::Client;
    bool _fShowEmptyBonuspackaMessage = false;
    friend class ImageProvider;

};

}
#endif // RC_MAINMODEL_H
