
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APIOBJECTSFACTORY_H
#define APIOBJECTSFACTORY_H

#include "iapiobjectsfactory.h"
#include <dbobjectsrequest.h>
#include <type_traits>
#include <isqldbcache.h>

namespace RC {


class APIObjectsFactory: public IAPIObjectsFactory
{
public:
    APIObjectsFactory(QH::ISqlDBCache *db);
protected:

#define check_type(Type) \
    static_assert(std::is_base_of_v<API::Type, Type>, \
    "The template argument must be base type of the API::"#Type)

    template<class User>
    QSharedPointer<API::User> getUserImpl(unsigned int userId) const {

        check_type(User);

        User request;
        request.setId(userId);

        return _db->getObject(request);
    }

    template<class UsersCards>
    QList<QSharedPointer<API::UsersCards> >getAllUserDataImpl(unsigned int userId) const {

        check_type(UsersCards);

        QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                      QString("user='%0'").arg(userId));

        auto result = _db->getObject(request);

        if (!result)
            return {};

        return {result->data().begin(), result->data().end()};
    };

    template<class UsersCards>
    QSharedPointer<API::UsersCards> getUserCardDataImpl(unsigned int userId,
                                                        unsigned int cardId) const {

        check_type(UsersCards);

        UsersCards request;
        request.setCard(cardId);
        request.setUser(userId);

        auto purches = _db->getObject(request);
        return purches;
    };

    template<class UsersCards>
    QList<QSharedPointer<API::UsersCards> > getAllUserFromCardImpl(unsigned int cardId) const {

        check_type(UsersCards);

        QString where = QString("card=%0").arg(cardId);

        QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards",
                                                      where);

        auto result = _db->getObject(request);

        if (!result)
            return {};

        return {result->data().begin(), result->data().end()};
    };

    template<class User>
    QList<QSharedPointer<API::User> > getAllUserDataFromCardImpl(unsigned int cardId) const {

        check_type(User);

        QString where = QString("card=%0").arg(cardId);

        where = QString("id IN (select user from UsersCards where %0)").arg(where);

        QH::PKG::DBObjectsRequest<User> request("Users",
                                                where);

        return _db->getObject(request)->data();
    };

    template<class Card>
    QSharedPointer<API::Card> getCardImpl(unsigned int cardId) {

        check_type(Card);

        Card request;
        request.setId(cardId);

        return _db->getObject(request);
    };

    template<class Card>
    QList<QSharedPointer<API::Card>> getAllUserCardsImpl(const QByteArray &userKey,
                                                         bool restOf = false) {

        check_type(Card);

        QString where = "ownerSignature= '%0'";
        if (restOf) {
            where = "ownerSignature!= '%0'";
        }

        where = where.arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding)));

        QH::PKG::DBObjectsRequest<Card> cardRequest("Cards", where);

        auto result = _db->getObject(cardRequest);

        if (!result)
            return {};

        return {result->data().begin(), result->data().end()};

    };

    template<class UsersCards>
    QList<QSharedPointer<API::UsersCards>> getAllUserCardsDataImpl(const QByteArray &userKey) {

        check_type(UsersCards);

        QString whereBlock = QString("card IN SELECT id FROM Cards WHERE ownerSignature = '%0'");
        QH::PKG::DBObjectsRequest<UsersCards>
                request("UsersCards",
                        whereBlock.arg(QString(userKey.toBase64(QByteArray::Base64UrlEncoding))));

        auto result = _db->getObject(request);

        if (!result)
            return {};

        return {result->data().begin(), result->data().end()};
    };

private:
    QH::ISqlDBCache *_db = nullptr;
};
}

#endif // APIOBJECTSFACTORY_H
