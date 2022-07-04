
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    property var waitModel: null

    padding: 10

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: title
        font.pointSize: 20
        color: (Material.theme === Material.Light)? "#424242" : "#aaaaaa"
        wrapMode: Label.WordWrap

    }

    Frame {
        id: waitForCondition
        visible : Boolean(waitModel && waitModel.waitConfirm)
        anchors.fill: parent

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
