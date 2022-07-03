//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef PERMISIONSMODEL_H
#define PERMISIONSMODEL_H

#include "userslistmodel.h"
#include <CheatCard/api/api0/contacts.h>
#include <QSharedPointer>

namespace RC {
class UserModel;
class WaitConfirmModel;

/**
 * @brief The PermisioonsModel class
 */
class PermisionsModel: public UsersListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* waitModel READ waitModel CONSTANT)

public:
    PermisionsModel(ImagesStorageModel *imageStorage);
    ~PermisionsModel() override;

    enum Roles {
        PermisionRole = Qt::UserRole
    };

    Q_INVOKABLE void setNewDescription(int row, const QString& description);
    Q_INVOKABLE void addNewPermision(const QString& description);
    Q_INVOKABLE void removePermision(int row);

    void setPermissions(const QList<QSharedPointer<API::Contacts> > &newData,
                        const QSharedPointer<API::User>& currentUser);

    QObject *waitModel() const;

    void handleServerResult(const QSharedPointer<RC::API::Contacts>& contact,
                            const QSharedPointer<API::User>& currentuser,
                            bool succesed, bool removed);

signals:
    void sigPermisionUpdated(const QSharedPointer<RC::API::Contacts>& permision);
    void sigPermisionRemoved(QSharedPointer<RC::API::Contacts> permision);
    void sigPermisionAdded(const QString& name);

private:
    QHash<unsigned int, QSharedPointer<RC::API::Contacts>> _data;
    QSharedPointer<RC::UserModel> _currentUserModel;
    WaitConfirmModel *_waitModel = nullptr;

};

}
#endif // PERMISIONSMODEL_H
