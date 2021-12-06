//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODE_H
#define BASENODE_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include <QSharedPointer>
#include <dbobject.h>
#include "datapack.h"
#include <isqldbcache.h>
#include <QSslError>


#define USERREQUEST_TIMEOUT 3
namespace RC {

class CardStatus;
class Card;
class UsersCards;
class CardDataRequest;
class CardStatusRequest;
class Session;
class User;
class ApplicationVersion;
class VersionIsReceived;

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(QH::ISqlDBCache *db);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData, unsigned int freeIndex) const;
    int getCountOfReceivedItems(unsigned int userId, unsigned int cardId);
    void removeCard(const QSharedPointer<Card> &objUserCard);

    int getCardFreeIndex(unsigned int cardId) const;
    unsigned int getCardVersion(unsigned int cardId) const;

    static QString libVersion();

    QSharedPointer<User>
    getUser(unsigned int userId) const;
    QList<QSharedPointer<UsersCards> >getAllUserData(unsigned int userId) const;

    QSharedPointer<UsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId) const;
    QList<QSharedPointer<UsersCards> >
    getAllUserFromCard(unsigned int cardId) const;

    QList<QSharedPointer<User> >
    getAllUserDataFromCard(unsigned int cardId) const;

    bool restoreOldData(const QByteArray &curentUserKey,
                        const QString& domain = "",
                        int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    QSharedPointer<Card> getCard(unsigned int cardId);

    /**
     * @brief getAllUserCards This method will return list of available cards of the user with @a userKey key
     * @param userKey user key
     * @param restOf This option force return list of not ovned cards.
     * @return cards list;
     */
    QList<QSharedPointer<Card>> getAllUserCards(const QByteArray &userKey,
                                                bool restOf = false);
    /**
     * @brief getAllUserCardsData This method will return list of available userscards data of the user with @a userKey key
     * @param userKey user key
     * @return cards data list;
     */
    QList<QSharedPointer<UsersCards>>
    getAllUserCardsData(const QByteArray &userKey);
    QByteArray getUserSecret(unsigned int userId) const;

    const QMap<int, QSharedPointer<QH::iParser> > &apiParsers() const;
    void setApiParsers(const QMap<int, QSharedPointer<QH::iParser> > &newApiParsers);
    void addApiParser(const QSharedPointer<QH::iParser>& );

    template<class AptType>
    void addApiParser() {
        addApiParser(QSharedPointer<AptType>::create(this));
    }

    QString getServerHost() const;
    /**
     * @brief cardValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    virtual bool cardValidation(const QSharedPointer<Card>& card,
                                const QByteArray &ownerSecret) const = 0;

    /**
     * @brief getSignData This method sets to @a data seecret key of this node. This method should be works only for sellers.
     * @param data result value.
     */
    virtual void getSignData(QByteArray& data) const = 0;

    QH::ISqlDBCache *db() const;

    const QSharedPointer<User> &currentUser() const;
    void setCurrentUser(QSharedPointer<User> newCurrentUser);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::UsersCards> data);
    void sigCardReceived(QSharedPointer<RC::Card> err);
    void sigNetworkError(QAbstractSocket::SocketError errorCode,
                         QSslError::SslError sslError);

protected:

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    bool processAppVersion(const QSharedPointer<ApplicationVersion> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool versionDeliveredSuccessful(const QSharedPointer<VersionIsReceived> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);

    QH::AbstractNodeInfo *createNodeInfo(QAbstractSocket *socket,
                                         const QH::HostAddress *clientAddress) const override;

    void nodeConnected(QH::AbstractNodeInfo *node) override;

    int maximumApiVersion() const;
    int minimumApiVersion() const;

    QSharedPointer<QH::iParser>
    selectParser(const ApplicationVersion& distVersion) const;


    // AbstractNode interface
protected slots:
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

    void handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) override;

private:
    QH::ISqlDBCache *_db = nullptr;

    QMap<int, QSharedPointer<QH::iParser>> _apiParsers;

    QSharedPointer<User> _currentUser;

};
}
#endif // BASENODE_H
