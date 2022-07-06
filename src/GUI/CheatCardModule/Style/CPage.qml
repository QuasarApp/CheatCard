
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
    id: root
    property var waitModel: null
    property bool wait: waitModel && waitModel.waitConfirm

    onWaitChanged: {
        if (wait) {
            waitPopUp.parent = this
            waitPopUp.open()
        }  else
            waitPopUp.close()
    }

    padding: 10

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: title
        font.pointSize: 20
        color: (Material.theme === Material.Light)? "#424242" : "#aaaaaa"
        wrapMode: Label.WordWrap

    }


    Popup {
        id: waitPopUp

        x: root.width / 2 - waitPopUp.width / 2
        y: root.height / 2 - waitPopUp.height / 2

        width: waitForCondition.implicitWidth + waitForCondition.padding * 2
        height:  waitForCondition.implicitHeight + waitForCondition.padding * 2

        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose

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
}
