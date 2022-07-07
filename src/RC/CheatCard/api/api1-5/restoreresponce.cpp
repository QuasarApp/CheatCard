#include "restoreresponce.h"

#include <CheatCard/api/api1/userscards.h>

#include <CheatCard/api/api0/contacts.h>

namespace RC {
namespace APIv1_5 {
RestoreResponce::RestoreResponce() {

}

bool RestoreResponce::isValid() const {
    return _userKey.size();
}

QDataStream &RestoreResponce::fromStream(QDataStream &stream) {
    stream >> _usersCards;
    stream >> _contacts;
    stream >> _userKey;

    return stream;
}

QDataStream &RestoreResponce::toStream(QDataStream &stream) const {
    stream << _usersCards;
    stream << _contacts;
    stream << _userKey;

    return stream;
}

const QByteArray &RestoreResponce::userKey() const {
    return _userKey;
}

void RestoreResponce::setUserKey(const QByteArray &newUserKey) {
    _userKey = newUserKey;
}

const QH::PKG::DataPack<API::Contacts> &RestoreResponce::contacts() const {
    return _contacts;
}

void RestoreResponce::setContacts(const QH::PKG::DataPack<API::Contacts> &newContacts) {
    _contacts = newContacts;
}

const QH::PKG::DataPack<APIv1::UsersCards> &RestoreResponce::usersCards() const {
    return _usersCards;
}

void RestoreResponce::setUsersCards(const QH::PKG::DataPack<APIv1::UsersCards> &newUsersCards) {
    _usersCards = newUsersCards;
}

}
}
