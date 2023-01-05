//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CREATECARDMODEL_H
#define CREATECARDMODEL_H

#include <QObject>
#include "cardmodel.h"

namespace RC {

namespace Interfaces {
class iCard;
class iDB;
class iUser;
}

class UsersListModel;

class CreateCardModel: public QObject, public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QObject * currentCard READ currentCard NOTIFY currentCardChanged)

public:
    CreateCardModel();
    ~CreateCardModel();
    QObject *currentCard() const;
    void init(const QSharedPointer<Interfaces::iDB> &db,
              const QSharedPointer<Interfaces::iModelsStorage> &global) override;


signals:
    void sigCardCreated(const QSharedPointer<Interfaces::iCard>& );
    void currentCardChanged();

private slots:
    void handleCardEditFinished(const QSharedPointer<Interfaces::iCard> &card);

private:
    void setCurrentCard(const QSharedPointer<Interfaces::iCard>&);
    QSharedPointer<Interfaces::iUser> getCurrentuser();

    CardModel* _currentCard = nullptr;
    QSharedPointer<UsersListModel> _usersModel;
};
}
#endif // CREATECARDMODEL_H
