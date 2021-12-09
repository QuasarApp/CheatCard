#ifndef IAPIOBJECTSFACTORY_H
#define IAPIOBJECTSFACTORY_H

#include <QSharedPointer>

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

/**
 * @brief The IAPIObjectsFactory class Contains functions for gettings objects from database.
 * Each methods contains own template implementation (method with the **impl** sufix). The IAPIObjectsFactory class has implementation for each api versions.
 */
class IAPIObjectsFactory
{
public:
    IAPIObjectsFactory() = default;
    virtual ~IAPIObjectsFactory() = default;

    virtual QSharedPointer<API::User> getUser(unsigned int userId) const = 0;
    virtual QList<QSharedPointer<API::UsersCards> >getAllUserData(unsigned int userId) const = 0;

    virtual QSharedPointer<API::UsersCards> getUserCardData(unsigned int userId,
                                                            unsigned int cardId) const = 0;

    virtual QList<QSharedPointer<API::UsersCards> > getAllUserFromCard(unsigned int cardId) const = 0;

    virtual QList<QSharedPointer<API::User> > getAllUserDataFromCard(unsigned int cardId) const = 0;
    virtual QSharedPointer<API::Card> getCard(unsigned int cardId) = 0;

    /**
     * @brief getAllUserCards This method will return list of available cards of the user with @a userKey key
     * @param userKey user key
     * @param restOf This option force return list of not ovned cards.
     * @return cards list;
     */
    virtual QList<QSharedPointer<API::Card>> getAllUserCards(const QByteArray &userKey,
                                                             bool restOf = false) = 0;
    /**
     * @brief getAllUserCardsData This method will return list of available userscards data of the user with @a userKey key
     * @param userKey user key
     * @return cards data list;
     */
    virtual QList<QSharedPointer<API::UsersCards>> getAllUserCardsData(const QByteArray &userKey) = 0;


};

}
#endif // IAPIOBJECTSFACTORY_H