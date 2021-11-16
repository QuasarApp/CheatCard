//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "CheatCard/dataconvertor.h"
#include "basenode.h"
#include "seller.h"
#include "CheatCard/userheader.h"

#include <CheatCard/session.h>
#include <CheatCard/user.h>
#include <CheatCard/userscards.h>

namespace RC {

Seller::Seller(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<CardStatusRequest>();
    registerPackageType<CardDataRequest>();
}

bool Seller::incrementPurchases(const QSharedPointer<UsersCards> &usersCardsData, int purchasesCount) {

    if (!usersCardsData)
        return false;

    if (purchasesCount < 0)
        return false;

    usersCardsData->setPurchasesNumber(usersCardsData->getPurchasesNumber() + purchasesCount);

    if (!db()->insertIfExistsUpdateObject(usersCardsData)) {
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCardsData);

    return true;
}

bool Seller::cardValidation(const QSharedPointer<Card> &card,
                             const QByteArray &ownerSecret) const {
    Q_UNUSED(card);
    Q_UNUSED(ownerSecret);

    return true;
}

void Seller::getSignData(QByteArray &data) const {
    if (currentUser())
        data = currentUser()->secret();
}

QString Seller::randomUserName() const {

    static QStringList list = {
        tr("Forget bag"),
        tr("Forget bag"),
        tr("Warty Warthog"),
        tr("Hoary Hedgehog"),
        tr("Breezy Badger"),
        tr("Dapper Drake"),
        tr("Edgy Eft"),
        tr("Feisty Fawn"),
        tr("Gutsy Gibbon"),
        tr("Hardy Heron"),
        tr("Intrepid Ibex"),
        tr("Jaunty Jackalope"),
        tr("Karmic Koala"),
        tr("Lucid Lynx"),
        tr("Maverick Meerkat"),
        tr("Natty Narwhal"),
        tr("Oneiric Ocelot"),
        tr("Precise Pangolin"),
        tr("Quantal Quetzal"),
        tr("Raring Ringtail"),
        tr("Saucy Salamander"),
        tr("Trusty Tahr"),
        tr("Utopic Unicorn"),
        tr("Vivid Vervet"),
        tr("Wily Werewolf"),
        tr("Xenial Xerus"),
        tr("Yakkety Yak"),
        tr("Zesty Zapus"),
        tr("Artful Aardvark"),
        tr("Bionic Beaver"),
        tr("Cosmic Cuttlefish"),
        tr("Disco Dingo"),
        tr("Eoan Ermine"),
        tr("Focal Fossa"),
        tr("Groovy Gorilla"),
        tr("Hirsute Hippo"),
        tr("Impish Indri")

    };

    return list[rand() % list.size()];
}

QSharedPointer<UsersCards> Seller::prepareData(const QSharedPointer<UserHeader> &userHeaderData,
                         unsigned int cardId) {

    if (!userHeaderData->isValid())
        return nullptr;

    auto session = QSharedPointer<Session>::create();

    session->setSessionId(userHeaderData->getSessionId());
    session->setUsercardId(UsersCards::genId(userHeaderData->getUserId(), cardId));

    if (!session->isValid()) {
        return nullptr;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    _lastRequested[session->getSessionId()] = session;

    User userrquest;
    userrquest.setId(userHeaderData->getUserId());

    auto dbUser = DataConvertor::toUser(userHeaderData);

    if (dbUser->name().isEmpty()) {
        dbUser->setName(randomUserName());
    }

    if (!db()->insertIfExistsUpdateObject(dbUser)) {
        QuasarAppUtils::Params::log("Failed to update user data", QuasarAppUtils::Warning);

    }

    auto userCardsData = getUserCardData(userHeaderData->getUserId(), cardId);
    if (!userCardsData) {
        userCardsData = QSharedPointer<UsersCards>::create();
        userCardsData->setOwner(false);
        userCardsData->setUser(userHeaderData->getUserId());
        userCardsData->setPurchasesNumber(0);
        userCardsData->setCard(cardId);
    }

    return userCardsData;
}

bool Seller::incrementPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                               unsigned int cardId, int purchasesCount,
                               const QString &domain, int port) {

    auto userCardsData = prepareData(userHeaderData, cardId);
    if (!userCardsData) {
        return false;
    }

    if (!incrementPurchases(userCardsData, purchasesCount)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);

        return false;
    }


    return addNode(domain, port);
}

bool Seller::sentDataToServerPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                                      unsigned int cardId,
                                      const QString &domain,
                                      int port) {

    auto userCardsData = prepareData(userHeaderData, cardId);
    if (!userCardsData) {
        return false;
    }

    return addNode(domain, port);
}

void Seller::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);

    for (const auto &session: qAsConst(_lastRequested)) {

        sendData(session.data(), node);
    }

    _lastRequested.clear();
}
}
