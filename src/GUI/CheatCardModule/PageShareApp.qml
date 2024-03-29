//#
//# Copyright (C) 2021 - 2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "Style"

CPage {
    id: root

    implicitHeight: 0x0
    implicitWidth: 0x0
    toolBarTitle: qsTr("Share Application")

    property int imageSize: 80

    contentItem:
        GridLayout {
            flow: (flowChecker.fHorisontal)? GridLayout.LeftToRight : GridLayout.TopToBottom

            implicitHeight: 0x0
            implicitWidth: 0x0

            GridLayout {
                implicitHeight: 0x0
                implicitWidth: 0x0

                columns: 2
                rows: 3

                ToolButton {
                    Layout.alignment:  Qt.AlignHCenter
                    icon.height: imageSize
                    icon.width: imageSize
                    Layout.columnSpan: 2
                    icon.color: Material.accent
                    icon.source: "qrc:/images/private/resources/Interface_icons/sharing_icon.svg"
                    onClicked: Qt.openUrlExternally("https://quasarapp.github.io/CheatCard/")
                }

                ToolButton {

                    icon.color: Material.accent
                    icon.source: "qrc:/images/private/resources/Interface_icons/copy.svg"

                    onClicked: {
                        ToolTip.show(qsTr("Copied in clipboard"))
                        mainModel.toClipboard(sharedUrl.text);
                    }
                }

                TextField {

                    id: sharedUrl
                    text: "https://quasarapp.github.io/CheatCard/"
                    Layout.fillWidth: true

                }

                Label {
                    Layout.columnSpan: 2

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
