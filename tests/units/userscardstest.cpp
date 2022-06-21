#include "userscardstest.h"

#include <CheatCard/api/api1/changeuserscards.h>
#include <CheatCard/api/api1/userscards.h>

UsersCardsTest::UsersCardsTest()
{

}

void UsersCardsTest::test() {
    const unsigned int user = 10;
    const unsigned int card = 20;

    RC::APIv1::ChangeUsersCards changes;
    auto usersCards = RC::API::UsersCards::genId(user, card);

    changes.setUsercardId(usersCards);

    QVERIFY(user == changes.getUser());
    QVERIFY(card == changes.getCard());

}
