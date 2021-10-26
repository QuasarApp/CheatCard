//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
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

    flow: (privateRoot.fHorisontal)? GridLayout.LeftToRight : GridLayout.TopToBottom

    property string model: ""

    Image {
        id: imgQr
        fillMode: Image.PreserveAspectFit
        Layout.preferredHeight: Math.min(parent.width * 0.8, parent.height * 0.8)
        Layout.preferredWidth:  Math.min(parent.width * 0.8, parent.height * 0.8)
        Layout.alignment: Qt.AlignHCenter
        source: "file:/" + model
        layer.enabled: true
        layer.effect: ShaderColorOverlay {
            color: Material.primary
            fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
        }
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
            text: qsTr("Show your phone the screen to the seller so that the displayed QR code is in the camera area of the seller's scanning device.");

            Layout.fillWidth: true
        }

        Label {
            wrapMode: Label.WordWrap
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("If the Seller's device completes the scan successfully, you can get your bonuses when you hide the QR code!");

            Layout.fillWidth: true
        }
    }

    Item {
        id: privateRoot

        property bool fHorisontal: root.height < root.width
    }
}
