//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ISALLERSUSERS_H
#define ISALLERSUSERS_H
#include <QSharedPointer>
#include "rci/global.h"

namespace RC {
namespace Interfaces {

/**
 * @brief The iContacts interface contains information of the extrnal permisions
 */
class CHEATCARD_INTERFACES_EXPORT iContacts
{
public:
    iContacts() = default;
    virtual ~iContacts() = default;

    /**
     * @brief isValid This method should be return true if this object is correct.
     * @return true if this object is correct else false.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief getInfo This methos should be return external information about contact (description)
     * @return description of the contact
     */
    virtual const QString &getInfo() const = 0;

    /**
     * @brief setInfo This method sets new description of the contact
     * @param newInfo This new value ofthe description of the contact
     */
    virtual void setInfo(const QString &newInfo) = 0;

    /**
     * @brief getUser This method return user id that create this contact.
     * @return user id that create this contact.
     */
    virtual unsigned int getUser() const = 0;

    /**
     * @brief getChildUserId This method return child user id
     * @return child user id
     */
    virtual unsigned int getChildUserId() const = 0;

    /**
     * @brief getChildUserKey This method return child user key.
     * @return child user key.
     */
    virtual const QByteArray &getChildUserKey() const = 0;

    /**
     * @brief setChildUserKey This method sets new value for child user key.
     * @param newChildUserKey This is new values of the child user key.
     */
    virtual void setChildUserKey(const QByteArray &newChildUserKey) = 0;

    /**
     * @brief getChildUserKey This method return main user key.
     * @return child user key.
     */
    virtual const QByteArray &getUserKey() const = 0;

    /**
     * @brief setChildUserKey This method sets new value for main user key.
     * @param newChildUserKey This is new values of the main user key.
     */
    virtual void setUserKey(const QByteArray &newUserKey) = 0;

};

}
}

Q_DECLARE_METATYPE(QSharedPointer<RC::Interfaces::iContacts>)
#endif // ISALLERSUSERS_H
