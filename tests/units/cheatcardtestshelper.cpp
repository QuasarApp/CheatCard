#include "cheatcardtestshelper.h"

#include <CheatCard/api/api0/user.h>

CheatCardTestsHelper::CheatCardTestsHelper()
{

}

QSharedPointer<RC::User> CheatCardTestsHelper::makeUser() {
    auto result = QSharedPointer<RC::User>::create();

    result->setName(QString("User%0").arg(rand()));

    return result;
}

unsigned int CheatCardTestsHelper::testCardId() {
    return 1990574875;
}

unsigned int CheatCardTestsHelper::testUserId() {
    return 2936319662;
}

QByteArray CheatCardTestsHelper::testUserPublicKey() {
    return QByteArray::fromBase64("OXLuc-QvtCj74ZgPPd4gujQvjv47vy78SSjXD8-O1Lo=",
                                  QByteArray::Base64UrlEncoding);
}

QByteArray CheatCardTestsHelper::testUserPrivKey() {
    return QByteArray::fromBase64("yj5JduAESMuQR4m-M1D7zcvm-rdCn-hlNF6m_-UijJk=",
                                  QByteArray::Base64UrlEncoding);
}

void softDeleteWrapDB(TestDataBaseWrapper *obj) {
    obj->softDelete();
}

void softDeleteWrapNode(RC::BaseNode *obj) {
    obj->softDelete();
}
