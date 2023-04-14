//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DOCTORPILLMODEL_H
#define DOCTORPILLMODEL_H

#include "models/basemodel.h"
#include <DoctorPillGui/doctormodel.h>

namespace RC {

/**
 * @brief The DoctorPillModel class is just wrapper of the DP::DoctorModel
 */
class DoctorPillModel: public DP::DoctorModel, public BaseModel
{
    Q_OBJECT
public:
    DoctorPillModel(const QList<QSharedPointer<DP::iPill> > &base);
};
}
#endif // DOCTORPILLMODEL_H
