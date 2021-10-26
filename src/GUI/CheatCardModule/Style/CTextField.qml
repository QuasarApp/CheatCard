//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


TextField {
    id: root
    property color fontColor: Material.foreground
    color: fontColor
    background: Rectangle {
        y: root.height - height - root.bottomPadding + 8
        implicitWidth: 120
        height: root.activeFocus || root.hovered ? 2 : 1
        color: root.activeFocus ? root.Material.accentColor
                                : (root.hovered ? root.Material.primaryTextColor : fontColor)
    }
}
