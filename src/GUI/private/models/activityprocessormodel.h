//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ACTIVITYPROCESSORMODEL_H
#define ACTIVITYPROCESSORMODEL_H

#include "basemodel.h"

#include <QObject>

namespace RC {

/**
 * @brief The ActivityProcessorModel class Is model of the Activity processor.
 *  Using This model you can show any qml view items from cpp code.
 */
class ActivityProcessorModel: public QObject, public BaseModel
{
    Q_OBJECT
public:
    ActivityProcessorModel();
    /**
     * @brief newActivity general nethod that open @a qmlView with @a model
     * @param qmlView This is path to file with implementation of qml widget.
     * @param model This is model of the view. By default nullptr.
     * @return true if widget show succeessful
     */
    bool newActivity(const QString &qmlView, QObject* model = nullptr);

    /**
     * @brief freeItem custom method for show availabel bonus page.
     * @param card This is card object that received bonus
     * @param userId This is user id that receive bonus
     * @param freeItemsCount This is count of the free bonuses
     * @return true if bonus page showed successfull
     */
    bool freeItem(QObject *card, const QByteArray &userId,
                  int freeItemsCount);

signals:
    void sigNewActivity(QString qmlView, QObject* model);
    void sigFreeItem(QObject *cardId, QByteArray userId,
                     int freeItemsCount);

};

}
#endif // ACTIVITYPROCESSORMODEL_H
