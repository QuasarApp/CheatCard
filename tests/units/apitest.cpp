#include "apitest.h"
#include "testserver.h"
#include "cheatcardtestshelper.h"

#include <CheatCard/api/apiv1-5.h>
#include <CheatCard/api/api1-5/deletecardrequest.h>
#include <CheatCard/api/api0/user.h>

template<class Base>
class ApiTester: public Base {
public:
    ApiTester(RC::BaseNode* node): Base(node) {}
    friend class APITest;
};

APITest::APITest() {

}

void APITest::test() {
    testProcessDeleteCardRequest();
}

void APITest::testProcessDeleteCardRequest() {
    auto server = CheatCardTestsHelper::makeNode<TestServer>(":/sql/units/sql/TestSallerDb.sql", true);
    server->addApiParser<ApiTester<RC::ApiV1_5>>();

    // api parser 1.5
    auto api = server->apiParsers().value(2).dynamicCast<ApiTester<RC::ApiV1_5>>();
    QVERIFY(api);

    auto request =  QSharedPointer<RC::APIv1_5::DeleteCardRequest>::create();
    request->setSecret("random");
    request->setCard(1990574875);

    // try remove card with invalid secreet. must be failed
    QVERIFY(!api->processDeleteCardRequest(request, nullptr, {}));

    auto user = server->getUser(2936319662);
    QVERIFY(user);
    request->setSecret(user->secret());

    // try remove card with correct secreet. must be failed
    QVERIFY(api->processDeleteCardRequest(request, nullptr, {}));
}


