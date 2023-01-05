//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef PERMISIONSMODEL_H
#define PERMISIONSMODEL_H

#include "userslistmodel.h"
#include <QSharedPointer>

namespace RC {
class UserModel;
class WaitConfirmModel;
class QRCodeDecoder;
class UserHeader;

namespace Interfaces {
class iContacts;
class iDB;
}
/**
 * @brief The PermisioonsModel class
 */
class PermisionsModel: public UsersListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* waitModel READ waitModel CONSTANT)

public:
    PermisionsModel();
    ~PermisionsModel() override;

    enum Roles {
        PermisionRole = Qt::UserRole
    };

    Q_INVOKABLE void setNewDescription(int row, const QString& description);
    Q_INVOKABLE void addNewPermision(const QString &rawUserHeaderData);
    Q_INVOKABLE void addNewPermisionFromImage(const QString &rawUserHeaderData);

    Q_INVOKABLE void removePermision(int row);

    void setPermissions(const QList<QSharedPointer<Interfaces::iContacts> > &newData);

    QObject *waitModel() const;

public slots:

    void handleServerResult(const QSharedPointer<RC::Interfaces::iContacts>& contact,
                            bool succesed, bool removed);

signals:
    void sigPermisionUpdated(const QSharedPointer<RC::Interfaces::iContacts>& permision);
    void sigPermisionRemoved(QSharedPointer<RC::Interfaces::iContacts> permision);
    void sigPermisionAdded(QSharedPointer<RC::UserHeader> userHeader);

private slots:
    void handleImageDecodet(const QString &data);

private:
    QSharedPointer<Interfaces::iUser>
    toUser(const QSharedPointer<Interfaces::iContacts> &contact);

    QHash<unsigned int, QSharedPointer<RC::Interfaces::iContacts>> _data;
    QSharedPointer<RC::UserModel> _currentUserModel;
    WaitConfirmModel *_waitModel = nullptr;
    QRCodeDecoder *_codeProcessor = nullptr;


};

}
#endif // PERMISIONSMODEL_H
