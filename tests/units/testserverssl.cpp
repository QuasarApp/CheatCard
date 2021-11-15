#include "testdatabasewrapper.h"
#include "testserverssl.h"

#include <QCoreApplication>

#include <CheatCard/card.h>
#include <CheatCard/userscards.h>


TestServerSSL::TestServerSSL(QSharedPointer<TestDataBaseWrapper> db):
    RC::ServerSSL(db->db()) {
    privateDb = db;

    QH::SslSrtData sslData;
    sslData.commonName = "localhost";
    sslData.organization = QCoreApplication::organizationName();

    useSelfSignedSslConfiguration(sslData);
}

QSharedPointer<RC::Card> TestServerSSL::getCard(unsigned int cardId) const {
    RC::Card card;
    card.setId(cardId);

    auto cardObj = db()->getObject(card);
    return cardObj;
}

int TestServerSSL::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
