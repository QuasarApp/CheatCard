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

GridLayout {
    id: root

    implicitHeight: 0x0
    implicitWidth: 0x0

    flow: (flowChecker.fHorisontal)? GridLayout.LeftToRight : GridLayout.TopToBottom

    property string model: ""

    QrCodeControl {
        Layout.preferredHeight: Math.min(parent.width * 0.8, parent.height * 0.8)
        Layout.preferredWidth:  Math.min(parent.width * 0.8, parent.height * 0.8)
        Layout.alignment: Qt.AlignHCenter
        customSource: "file:/" + model
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Label {
            wrapMode: Label.WordWrap
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("It looks like this code should be shown to the seller!");
            font.bold: true
            Layout.fillWidth: true
        }

        Label {
            text: " "
        }

        Label {
            wrapMode: Label.WordWrap
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("Show your phone to the seller, so that the drawn QR code gets into the scanning area.");

            Layout.fillWidth: true
        }

        Label {
            wrapMode: Label.WordWrap
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("When all passed is successful, the Seller's device will complete the scan, and you will receive your bonuses as soon as you hide the Qr code.");

            Layout.fillWidth: true
        }
    }
}
