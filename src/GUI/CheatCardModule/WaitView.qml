//#
//# Copyright (C) 2023 - 2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Popup {
    id: root
    property var model: null

    x: parent.width / 2 - root.width / 2
    y: parent.height / 2 - root.height / 2

    width: waitForCondition.implicitWidth + waitForCondition.padding * 2
    height:  waitForCondition.implicitHeight + waitForCondition.padding * 2

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    visible: model && model.waitConfirm

    contentItem:
    Pane {
        id: waitForCondition
        contentItem: ColumnLayout {
            Label {
                text: qsTr("Please wait for response from server");
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
            }

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                running: true;
            }
        }
    }
}

