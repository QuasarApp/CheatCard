//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


ToolButton {
    property var networkStatus: (mainModel)? mainModel.getNetIndicatorModel(): null
    property string onlineMode: "qrc:/images/private/resources/Interface_icons/online_icon.svg"
    property string offlineMode: "qrc:/images/private/resources/Interface_icons/offline_icon.svg"

    id: networkind
    icon.source: (networkStatus && networkStatus.enableNetwork)? onlineMode: offlineMode
}
