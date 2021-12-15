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
#include <CheatCard/api/apiobjectsfactoryv1.h>
#include "nodetype.h"

#define USERREQUEST_TIMEOUT 3
namespace RC {

namespace API {
class Card;
class UsersCards;
class Session;
class User;
class CardStatus;
class CardDataRequest;
class CardStatusRequest;
}

class ApplicationVersion;
class VersionIsReceived;

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode, public APIObjectsFactoryV1
{
    Q_OBJECT
public:
    BaseNode(QH::ISqlDBCache *db);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId) const;
    int getFreeItemsCount(const QSharedPointer<API::UsersCards>& inputData) const;
    int getFreeItemsCount(const QSharedPointer<API::UsersCards>& inputData, unsigned int freeIndex) const;
    int getCountOfReceivedItems(unsigned int userId, unsigned int cardId);
    void removeCard(const QSharedPointer<API::Card> &objUserCard);

    int getCardFreeIndex(unsigned int cardId) const;
    unsigned int getCardVersion(unsigned int cardId) const;

    static QString libVersion();

    bool restoreOldData(const QByteArray &curentUserKey,
                        const QString& domain = "",
                        int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool restoreOneCard(unsigned int cardId,
                        const QString& domain = "",
                        int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    QByteArray getUserSecret(unsigned int userId) const;

    QSharedPointer<QH::iParser> getSelectedApiParser(QH::AbstractNodeInfo *node) const;

    const QMap<int, QSharedPointer<QH::iParser> > &apiParsers() const;
    void setApiParsers(const QMap<int, QSharedPointer<QH::iParser> > &newApiParsers);
    void addApiParser(const QSharedPointer<QH::iParser>& );

    template<class AptType>
    void addApiParser() {
        addApiParser(QSharedPointer<AptType>::create(this));
    }

    QString getServerHost() const;


    /**
     * @brief nodeType This method should be return node type. The node type can be used in api for separete logics.
     * @return
     */
    virtual NodeType nodeType() const = 0;

    QH::ISqlDBCache *db() const;

    const QSharedPointer<API::User> &currentUser() const;
    void setCurrentUser(QSharedPointer<API::User> newCurrentUser);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::API::UsersCards> data);
    void sigCardReceived(QSharedPointer<RC::API::Card> err);
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

    QSharedPointer<API::User> _currentUser;

};
}
#endif // BASENODE_H
