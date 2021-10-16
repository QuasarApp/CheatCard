//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "aboutmodel.h"
#include "heart.h"
#include "CheatCard/basenode.h"

namespace RC {

AboutModel::AboutModel()
{

}

QString AboutModel::getCoreLibVr() const {
    return RC::BaseNode::libVersion();
}

QString AboutModel::getHeartLibVr() const {
    return QH::heartLibVersion();
}

QString AboutModel::getZxingCppLibVr() const {
    return "";
}

QString AboutModel::getSCodesLibVr() const {
    return "";
}

}
