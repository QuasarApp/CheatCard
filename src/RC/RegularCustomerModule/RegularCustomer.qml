//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visible: true
    height: 800
    width: 600

    property var model: mainModel

    Dialog {
        id: firstRun;
        visible: (model)? model.fFirst : false

        closePolicy: Popup.NoAutoClose

        height: mainWindow.height * 0.95
        width: mainWindow.width * 0.95
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2

        FirstRunPage {
            anchors.fill: parent
            model: mainModel
            onFinished: () => {
                firstRun.close()
            }
        }
    }
}
