//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USER_V3_H
#define USER_V3_H
#include <abstractdata.h>


namespace RC {
namespace API {
namespace V3 {

/**
 * @brief The User class This is maic class for contatins all information about user
 * The user structure is:
 * Secreet - value with that user will be prove to the server that it is owner.
 * User Key - this is hash of secret value (unique user key)
 * user id - short user id using for database key.
 *
 * ## Generate seteps:
 *
 * make secret
 * from secrete make key
 * from key make id.
 */
class User: public QH::PKG::AbstractData
{
    QH_PACKAGE_AUTO(API::V3::User)

public:
    User();

    bool isValid() const override;
    bool isAllKeysIsValid() const;

    const QString &name() const;
    void setName(const QString &newName);

    bool fSaller() const;
    void setFSaller(bool newFSaller);

    const QByteArray &getKey() const;
    void setKey(const QByteArray &newKey);

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

    void regenerateKeys();

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:

    QByteArray randomArray() const;

    QByteArray _key;
    QByteArray _secret;

    QString _name;
    bool _fSaller = false;
};
}
}
}
#endif // USER_H
