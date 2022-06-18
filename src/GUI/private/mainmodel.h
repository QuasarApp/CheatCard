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


namespace DP {
class DoctorModel;
}

namespace RC {


namespace API {
class Card;
class User;
class UsersCards;
class UserHeader;

}

class SoundPlayback;
class NetIndicatorModel;
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
class LanguagesModel;
class ActivityProcessorModel;
class CreateCardModel;
class UsersListModel;
class ImagesStorageModel;

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
    Q_PROPERTY(QObject * doctorModel READ doctorModel NOTIFY doctorModelChanged)
    Q_PROPERTY(QObject * langModel READ langModel NOTIFY langModelChanged)
    Q_PROPERTY(QObject * activityProcessorModel READ activityProcessorModel NOTIFY activityProcessorModelChanged)
    Q_PROPERTY(QObject * createCardModel READ createCardModel NOTIFY createCardModelChanged)
    Q_PROPERTY(QObject * usersListModel READ usersListModel NOTIFY usersListModelChanged)


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
    Q_INVOKABLE QObject *getNetIndicatorModel() const;
    QObject *currentUser() const;

    const QSharedPointer<UserModel>& getCurrentUser() const;

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
    Q_INVOKABLE bool fBillingAwailable() const;
    Q_INVOKABLE QString storeLink() const;

    QObject *statisticModel() const;

    QObject *exportImportModel() const;

    QObject *doctorModel() const;
    QObject *langModel() const;
    QObject *activityProcessorModel() const;
    QObject *createCardModel() const;

    QObject *usersListModel() const;

public slots:
    void setCurrentUser(const QSharedPointer<RC::UserModel> &newCurrentUser);

    void handleFirstDataSendet();
    void handleBonusGivOut(int userId, int cardId, int givOutcount);

signals:

    void fFirstChanged();
    void currentUserChanged();

    void cardsListChanged();

    void defaultLogosModelChanged();

    void defaultBackgroundsModelChanged();

    void modeChanged();

    void waitModelChanged();
    void statisticModelChanged();

    void exportImportModelChanged();

    void doctorModelChanged();
    void langModelChanged();
    void activityProcessorModelChanged();

    // SettingsListner interface
    void createCardModelChanged();

    void usersListModelChanged();

protected:
    void handleSettingsChanged(const QString &key, const QVariant &value) override;

private slots:
    bool handleImportUser(const QString &base64UserData);
    void handleCardReceived(QSharedPointer<RC::API::Card> card);

    void handleCardEditFinished(const QSharedPointer<RC::API::Card> &card);
    void handleResetCardModel(const QSharedPointer<RC::API::Card> &card);

    void handleRemoveRequest(const QSharedPointer<API::Card> &card);
    void handleCardSelectedForWork(const QSharedPointer<RC::CardModel>& card);
    void handleCardSelectedForStatistic(const QSharedPointer<RC::CardModel>& card);

    void handlePurchaseWasSuccessful(QSharedPointer<API::UsersCards>, bool alert);
    void handleListenStart(int purchasesCount, QSharedPointer<RC::CardModel> model, const QString &extraData);
    void handleListenStop();
    void handleAppStateChanged(Qt::ApplicationState state);
    void handlePurchaseReceived(RC::Purchase purchase);
    void saveCard(const QSharedPointer<RC::API::Card> &card);
    void handleCardCreated(const QSharedPointer<API::Card> &card);
    void handleAppOutdated(int minimumRequiredVersion);

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
    void initNetIndicateModels();
    void initDoctorModel();
    void initLanguageModel();
    void initActivityProcessorModel();
    void initCreateCardModel();
    void initUsersListModel();
    void initBackgroundsModel();
    void initIconsModel();

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
    UsersListModel *_usersListModel = nullptr;
    ImagesStorageModel *_backgrounds = nullptr;
    ImagesStorageModel *_icons = nullptr;


    AboutModel *_aboutModel = nullptr;
    SoundPlayback *_soundEffect = nullptr;
    SellerStatisticModel *_statisticModel = nullptr;
    ItemsModel *_defaultLogosModel = nullptr;
    ItemsModel *_defaultBackgroundsModel = nullptr;
    NetIndicatorModel *_netIdicatorModel = nullptr;
    DP::DoctorModel *_doctorModel = nullptr;
    LanguagesModel *_langModel = nullptr;
    ActivityProcessorModel *_activityProcessorModel = nullptr;
    CreateCardModel *_createCardModel = nullptr;

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
