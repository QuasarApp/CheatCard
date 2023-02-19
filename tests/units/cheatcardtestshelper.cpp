#include "cheatcardtestshelper.h"

CheatCardTestsHelper::CheatCardTestsHelper()
{

}

QSharedPointer<RC::Interfaces::iUser> CheatCardTestsHelper::makeUser() {
    static auto db = RC::DB::makeDb(1);
    auto newUser = db->makeEmptyUser();

    newUser->setName(QString("User%0").arg(rand()));
    return newUser;
}

QSharedPointer<RC::Interfaces::iContacts> CheatCardTestsHelper::makeContact() {
    static auto db = RC::DB::makeDb(1);
    return db->makeEmptyContact();
}

QByteArray CheatCardTestsHelper::testCardId() {
    return "1990574875";
}

QByteArray CheatCardTestsHelper::testUserPublicKey() {
    return QByteArray::fromBase64("OXLuc-QvtCj74ZgPPd4gujQvjv47vy78SSjXD8-O1Lo=",
                                  QByteArray::Base64UrlEncoding);
}

QByteArray CheatCardTestsHelper::testUserPrivKey() {
    return QByteArray::fromBase64("yj5JduAESMuQR4m-M1D7zcvm-rdCn-hlNF6m_-UijJk=",
                                  QByteArray::Base64UrlEncoding);
}

void softDeleteWrapNode(RC::BaseNode *obj) {
    obj->softDelete();
}
