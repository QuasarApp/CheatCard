//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "doctorpillmodel.h"

namespace RC {

DoctorPillModel::DoctorPillModel(const QList<QSharedPointer<DP::iPill> > &base):
    DP::DoctorModel(base){}
}
