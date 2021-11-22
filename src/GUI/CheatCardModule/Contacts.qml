//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Page {
    id: root

    contentItem: ColumnLayout {

        Frame {

            background: Rectangle{
                color: "#e6e6e6"
                radius: 10
            }

            Layout.margins: 8
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            contentItem: RowLayout {
                implicitWidth: 0x0

                ToolButton {
                    icon.source: "qrc:/images/private/resources/platformsIcon/slack-logo.svg"
                    icon.height: 80
                    icon.width: 80
                    icon.color: "transparent"
                    onClicked: Qt.openUrlExternally("https://join.slack.com/t/slack-uvu5934/shared_invite/zt-xtxp3txy-z4E_8mFsabqSRIzy7AwJXg")
                }

                Text {
                    text: qsTr("Join to our slack channel for get support and see the last changes.")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap

                    Layout.fillWidth: true
                }
            }
        }

        Frame {

            background: Rectangle{
                color: "#e6e6e6"
                radius: 10
            }

            Layout.margins: 8
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            contentItem: RowLayout {
                implicitWidth: 0x0

                ToolButton {
                    icon.height: 80
                    icon.width: 80
                    icon.color: "transparent"
                    icon.source: "qrc:/images/private/resources/platformsIcon/telegram.svg"
                    onClicked: Qt.openUrlExternally("https://t.me/joinchat/Msv_LWw4GI4zNTIy")
                }

                Text {
                    text: qsTr("Join to our telegram channel for get support and see the last changes.")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap

                    Layout.fillWidth: true
                }
            }
        }

        Frame {

            background: Rectangle{
                color: "#e6e6e6"
                radius: 10
            }

            Layout.margins: 8
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            contentItem: RowLayout {
                implicitWidth: 0x0

                ToolButton {
                    icon.height: 80
                    icon.width: 80
                    icon.color: "transparent"
                    icon.source: "qrc:/images/private/resources/platformsIcon/Instagram.svg"
                    onClicked: Qt.openUrlExternally("https://www.instagram.com/QuasarApp.CheatCard/")
                }

                Text {
                    text: qsTr("Join to our Instagram channel for get support and see the last changes.")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap

                    Layout.fillWidth: true
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

    }
}
