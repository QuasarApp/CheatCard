//#
//# Copyright (C) 2021 - 2022 QuasarApp.
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

    implicitHeight: 0x0
    implicitWidth: 0x0

    property int imageSize: 80

    contentItem:
        GridLayout {
            flow: (flowChecker.fHorisontal)? GridLayout.LeftToRight : GridLayout.TopToBottom

            implicitHeight: 0x0
            implicitWidth: 0x0

            ColumnLayout {
                implicitHeight: 0x0
                implicitWidth: 0x0

                Layout.fillWidth: true
                Layout.fillHeight: true

                ToolButton {
                    Layout.alignment:  Qt.AlignHCenter
                    icon.height: imageSize
                    icon.width: imageSize

                    icon.color: Material.accent
                    icon.source: "qrc:/images/private/resources/Interface_icons/sharing_icon.svg"
                    onClicked: Qt.openUrlExternally("https://quasarapp.github.io/CheatCard/")
                }

                Label {
                    text: qsTr("To get a cheat card, scan the QR code and follow the link.")
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignTop
                    wrapMode: Label.WordWrap
                    font.pointSize: 14

                    Layout.fillWidth: true
                }
            }

            Image {
                asynchronous: true
                fillMode: Image.PreserveAspectFit
                Layout.preferredHeight: Math.min(root.width * 0.8, root.height * 0.8)
                Layout.preferredWidth:  Math.min(root.width * 0.8, root.height * 0.8)
                Layout.alignment: Qt.AlignHCenter
                source: "qrc:/images/private/resources/Interface_icons/shareApp.png"
                layer.enabled: true
                layer.effect: ShaderColorOverlay {
                    color: Material.primary
                    fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
                }
            }

        }
}
