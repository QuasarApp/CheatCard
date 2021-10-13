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

QString AboutModel::getCoreLibVr() {
    return RC::BaseNode::libVersion();
}

QString AboutModel::getHeartLibVr() {
    return QH::heartLibVersion();
}

QString AboutModel::getZxingCppLibVr() {
    return "<a href='https://github.com/nu-book/zxing-cpp'>ZXing-C++</a>";
}

QString AboutModel::getSCodesLibVr() {
    return "<a href='https://github.com/scytheStudio/SCodes'>SCodes</a>";
}

}
