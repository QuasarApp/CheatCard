//#
//# Copyright (C) 2022 - 2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


Label {

    elide: Label.ElideMiddle
    Layout.fillWidth: true
    ToolTip.text: text
    ToolTip.visible: privateRoot.hold

    MouseArea {
        id: privateRoot

        property bool hold: false

        onPressed: {
            hold = true
        }

        onReleased: {
            hold = false
        }

        onExited: {
            hold = false
        }

        anchors.fill: parent;
    }
}
