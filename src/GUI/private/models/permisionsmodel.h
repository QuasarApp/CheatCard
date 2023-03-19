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
class QRCodeDecoder;
class UserHeader;

namespace Interfaces {
class iContacts;
class iDB;
}
/**
 * @brief The PermisioonsModel class
 */
class PermisionsModel:  public QAbstractListModel, public BaseModel
{
    Q_OBJECT

public:
    PermisionsModel();
    ~PermisionsModel() override;

    enum Roles {
        PermisionDescriptionRole = Qt::UserRole,
        DefaultAvatar,

    };

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setNewDescription(QByteArray userKey, const QString& description);
    Q_INVOKABLE void addNewPermision(const QString &rawUserHeaderData);
    Q_INVOKABLE void addNewPermisionFromImage(const QString &rawUserHeaderData);

    Q_INVOKABLE void removePermision(QByteArray userKey);

    void init(const QSharedPointer<Interfaces::iDB> &db,
              const QSharedPointer<Interfaces::iModelsStorage> &global) override;

public slots:
    void setPermissions(const QList<QSharedPointer<Interfaces::iContacts> > &newData);

private slots:
    void handleImageDecodet(const QString &data);
    void refreshTable(QByteArray userKey);

private:
    void removePermisionPrivate(QByteArray permision);
    void addNewPermisionPrivate(QSharedPointer<RC::Interfaces::iContacts> permision);

    QSharedPointer<Interfaces::iUser>
    toUser(const QSharedPointer<Interfaces::iContacts> &contact);

    QList<QByteArray> _permissions;
    QHash<QByteArray, QSharedPointer<RC::Interfaces::iContacts>> _data;
    QSharedPointer<RC::UserModel> _currentUserModel;
    QRCodeDecoder *_codeProcessor = nullptr;
    QSharedPointer<ImagesStorageModel> _defaultAvatars;


};

}
#endif // PERMISIONSMODEL_H
