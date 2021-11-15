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
#include "datapack.h"
#include <isqldbcache.h>

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

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(QH::ISqlDBCache *db);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData, unsigned int freeIndex) const;
    int getCountOfReceivedItems(unsigned int userId, unsigned int cardId);

    int getCardFreeIndex(unsigned int cardId) const;
    unsigned int getCardVersion(unsigned int cardId) const;

    static QString libVersion();

    QSharedPointer<UsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId) const;
    QList<QSharedPointer<UsersCards> >
    getAllUserFromCard(unsigned int cardId, bool includeOwner = true) const;

    QList<QSharedPointer<User> >
    getAllUserDataFromCard(unsigned int cardId, bool includeOwner = true) const;

    QSharedPointer<Card> getCard(unsigned int cardId);

    const QMap<int, QSharedPointer<QH::iParser> > &apiParsers() const;
    void setApiParsers(const QMap<int, QSharedPointer<QH::iParser> > &newApiParsers);
    void addApiParser(const QSharedPointer<QH::iParser>& );

    template<class AptType>
    void addApiParser() {
        addApiParser(QSharedPointer<AptType>::create(this));
    }


    /**
     * @brief cardValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    virtual bool cardValidation(const QSharedPointer<Card>& card,
                                const QByteArray &ownerSecret) const = 0;

    QH::ISqlDBCache *db() const;

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::UsersCards> data);
    void sigCardReceived(QSharedPointer<RC::Card> err);


protected:

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    bool processAppVersion(const QSharedPointer<ApplicationVersion> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);


    QH::AbstractNodeInfo *createNodeInfo(QAbstractSocket *socket,
                                         const QH::HostAddress *clientAddress) const override;

    void nodeConnected(QH::AbstractNodeInfo *node) override;

private:
    QH::ISqlDBCache *_db = nullptr;

    QMap<int, QSharedPointer<QH::iParser>> _apiParsers;
};
}
#endif // BASENODE_H
