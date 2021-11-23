//#
//# Copyright (C) 2021 - 2021 QuasarApp.
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

    contentItem: ColumnLayout {

        Text {
            text: qsTr("To get a cheat card, scan the QR code and follow the link.")
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            wrapMode: Text.WordWrap

            Layout.fillWidth: true
        }

        Image {
            id: imgQr
            fillMode: Image.PreserveAspectFit
            Layout.preferredHeight: Math.min(parent.width * 0.9, parent.height * 0.9)
            Layout.preferredWidth:  Math.min(parent.width * 0.9, parent.height * 0.9)
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:/images/private/resources/Interface_icons/share_url_qrcode.png"
            layer.enabled: true
            layer.effect: ShaderColorOverlay {
                color: Material.primary
                fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
            }
        }



    }
}
