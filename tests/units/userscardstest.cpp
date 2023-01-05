#include "userscardstest.h"

#include <rci/rcutils.h>

UsersCardsTest::UsersCardsTest()
{

}

void UsersCardsTest::test() {
    const unsigned int user = 10;
    const unsigned int card = 20;

    auto usersCards = RC::RCUtils::makeUsersCardsId(user, card);

    QVERIFY(user == RC::RCUtils::getUserIdFromUsrsCards(usersCards));
    QVERIFY(card == RC::RCUtils::getCardIdFromUsrsCards(usersCards));

}
