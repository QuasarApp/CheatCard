//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RC_MAINMODEL_H
#define RC_MAINMODEL_H

#include <QObject>
#include <QSettings>
#include <modelsstorage.h>
#include <settingslistner.h>
#include <CheatCardGui/ibilling.h>
#include <rci/core/idb.h>


namespace DP {
class DoctorModel;
}

namespace RC {

namespace Interfaces {
class iCard;
class iUsersCards;
}

class SoundPlayback;
class NetIndicatorModel;
class CardModel;
class AboutModel;
class CardsListModel;
class UserModel;
class ItemsModel;
class IncomeModel;
class WaitModel;
class Client;
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
class PermisionsModel;
class UserHeader;

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject, public QuasarAppUtils::SettingsListner
{
    Q_OBJECT
    Q_PROPERTY(bool fFirst READ fFirst NOTIFY fFirstChanged)

    Q_PROPERTY(QObject * cardsList READ cardsList NOTIFY cardsListChanged)
    Q_PROPERTY(int mode READ getMode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(QObject * defaultLogosModel READ defaultLogosModel NOTIFY defaultLogosModelChanged)
    Q_PROPERTY(QObject * defaultBackgroundsModel READ defaultBackgroundsModel NOTIFY defaultBackgroundsModelChanged)
    Q_PROPERTY(QObject * incomeModel READ incomeModel NOTIFY incomeModelChanged)
    Q_PROPERTY(QObject * waitModel READ waitModel NOTIFY waitModelChanged)

    Q_PROPERTY(QObject * statisticModel READ statisticModel NOTIFY statisticModelChanged)
    Q_PROPERTY(QObject * exportImportModel READ exportImportModel NOTIFY exportImportModelChanged)
    Q_PROPERTY(QObject * doctorModel READ doctorModel NOTIFY doctorModelChanged)
    Q_PROPERTY(QObject * langModel READ langModel NOTIFY langModelChanged)
    Q_PROPERTY(QObject * activityProcessorModel READ activityProcessorModel NOTIFY activityProcessorModelChanged)
    Q_PROPERTY(QObject * createCardModel READ createCardModel NOTIFY createCardModelChanged)
    Q_PROPERTY(QObject * usersListModel READ usersListModel NOTIFY usersListModelChanged)
    Q_PROPERTY(QObject * permisionsModel READ permisionsModel CONSTANT)


public:
    enum class Mode {
        Unknown = -1,
        Client = 0,
        Seller = 1
    };
    MainModel(const QSharedPointer<Interfaces::iDB> &db);
    ~MainModel();

    bool fFirst() const;
    Q_INVOKABLE void configureFinished();
    Q_INVOKABLE QObject *getAboutModel();
    Q_INVOKABLE QObject *getNetIndicatorModel() const;

    QObject *cardsList() const;
    QObject *defaultLogosModel() const;
    QObject *defaultBackgroundsModel() const;

    int getMode() const;
    void setMode(int newMode);

    QObject *incomeModel() const;

    void initBilling(IBilling* billingObject);

    /**
     * @brief flush Save all temp users data and configuration
     */
    void flush();

    Q_INVOKABLE bool fBillingAwailable() const;
    Q_INVOKABLE QString storeLink() const;

    QObject *statisticModel() const;

    QObject *exportImportModel() const;

    QObject *doctorModel() const;
    QObject *langModel() const;
    QObject *activityProcessorModel() const;
    QObject *createCardModel() const;

    QObject *usersListModel() const;
    QObject *permisionsModel() const;

    QObject *waitModel() const;

public slots:
    void handleCurrentUserChanged(const QSharedPointer<RC::UserModel> &newCurrentUser);
    void handleBonusGivOut(QByteArray userId, QByteArray cardId, int givOutcount);

signals:

    void currentUserChanged();

    void cardsListChanged();

    void defaultLogosModelChanged();

    void defaultBackgroundsModelChanged();

    void modeChanged();

    void incomeModelChanged();
    void statisticModelChanged();

    void exportImportModelChanged();

    void doctorModelChanged();
    void langModelChanged();
    void activityProcessorModelChanged();

    // SettingsListner interface
    void createCardModelChanged();

    void usersListModelChanged();
    void fFirstChanged();

    void waitModelChanged();

protected:
    void handleSettingsChanged(const QString &key, const QVariant &value) override;
    QByteArray importDeprecatedUser(const QString &base64UserData);

private slots:
    bool handleImportUser(const QString &base64UserData);

    void handleCardReceived(QSharedPointer<RC::Interfaces::iCard> card);

    void handleCardEditFinished(const QSharedPointer<RC::Interfaces::iCard> &card);
    void handleResetCardModel(const QSharedPointer<RC::Interfaces::iCard> &card);

    void handleRemoveRequest(const QSharedPointer<RC::Interfaces::iCard> &card);
    void handleCardSelectedForWork(const QSharedPointer<RC::CardModel>& card);
    void handleCardSelectedForStatistic(const QSharedPointer<RC::CardModel>& card);

    void handlePurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards>, bool alert);
    void handleListenStart(int purchasesCount,
                           QSharedPointer<RC::CardModel> model,
                           QSharedPointer<RC::UserHeader>);
    void handleListenStop();
    void handleAppStateChanged(Qt::ApplicationState state);
    void handlePurchaseReceived(RC::Purchase purchase);
    void saveCard(const QSharedPointer<RC::Interfaces::iCard> &card);
    void handleCardCreated(const QSharedPointer<RC::Interfaces::iCard> &card);
    void handleAppOutdated(int minimumRequiredVersion);
    void handlePermissionChanged(const QSharedPointer<RC::Interfaces::iContacts>& permision);
    void handlePermissionRemoved(QSharedPointer<RC::Interfaces::iContacts> permision);
    void handlePermissionAdded(QSharedPointer<RC::UserHeader> childUserName);
    void handleContactsListChanged();
    void handleSerrverSentError(unsigned char code, QString);

private:
    void lastStatusRequest();

    const QSharedPointer<Interfaces::iDB>& db() const;

    void initModels();

    void configureCardsList();

    void setCardListModel(const QSharedPointer<CardsListModel>& model);

    void initMode(const QSharedPointer<UserModel>& user);

    void soundEffectPlayback(const QString &soundName);

    bool sendSellerDataToServer(const QByteArray &userKey,
                                const QByteArray &cardId,
                                int purchasesCount,
                                bool receive);

    CardsListModel* getCurrentListModel() const;
    void setFirst(bool ffirst);
    void initCardsListModels();
    void initImagesModels();

    bool _firstRun = true;

    QSharedPointer<Interfaces::iDB> _db;
    SettingsModel* _config = nullptr;

    QSharedPointer<CardsListModel> _cardsListModel;
    QSharedPointer<CardsListModel> _ownCardsListModel;
    QSharedPointer<CardProxyModel> _currentCardsListModel;

    SoundPlayback *_soundEffect = nullptr;
    QSharedPointer<ItemsModel> _defaultLogosModel;
    QSharedPointer<ItemsModel> _defaultBackgroundsModel;

    QSharedPointer<ModelsStorage> _modelStorage;

    IBilling *_billing = nullptr;

    QByteArray _lastUserKey;

    Mode _mode = Mode::Client;
    bool _fShowEmptyBonuspackaMessage = false;
    QByteArray _currentUserKey;
    friend class ImageProvider;
};

}
#endif // RC_MAINMODEL_H
