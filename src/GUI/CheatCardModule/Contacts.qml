//#
//# Copyright (C) 2018 - 2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "Style"

CPage {
    id: rootContacts

    property int imageSize: 80

    implicitHeight: listContacts.height + rootContacts.padding * 2
    toolBarTitle: qsTr("Contact with developers")

    background: Rectangle {
        color: "transparent"
    }

    contentItem: Flickable {
        id: flickable
        ScrollBar.vertical: ScrollBar {}
        contentHeight: listContacts.height
        clip: true


        ColumnLayout {
            id: listContacts
            width: flickable.width            

            Frame {

                background: Rectangle {
                    color: "transparent"
                    border.color: rootContacts.palette.mid
                    radius: 10
                }

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: RowLayout {
                    implicitWidth: 0x0

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Logo/CheatCard_Logo1.png"
                        icon.height: imageSize
                        icon.width: imageSize
                        icon.color: "transparent"
                        onClicked: Qt.openUrlExternally("https://github.com/QuasarApp/CheatCard/discussions")
                    }

                    Label {
                        text: qsTr("Go to discussions. There you will get extended help.")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap

                        Layout.fillWidth: true
                    }
                }
            }

            Frame {

                background: Rectangle {
                    color: "transparent"
                    border.color: rootContacts.palette.mid
                    radius: 10
                }

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: RowLayout {
                    implicitWidth: 0x0

                    ToolButton {
                        icon.height: imageSize
                        icon.width: imageSize
                        icon.color: "transparent"
                        icon.source: "qrc:/images/private/resources/platformsIcon/telegram.svg"
                        onClicked: Qt.openUrlExternally("https://t.me/cheatcard")
                    }

                    Label {
                        text: qsTr("Join to our telegram channel to get support and see the last changes.")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap

                        Layout.fillWidth: true
                    }
                }
            }

            Frame {

                background: Rectangle {
                    color: "transparent"
                    border.color: rootContacts.palette.mid
                    radius: 10
                }

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: RowLayout {
                    implicitWidth: 0x0

                    ToolButton {
                        icon.height: imageSize
                        icon.width: imageSize
                        icon.color: "transparent"
                        icon.source: "qrc:/images/private/resources/platformsIcon/Instagram.svg"
                        onClicked: Qt.openUrlExternally("https://www.instagram.com/QuasarApp.CheatCard/")
                    }

                    Label {
                        text: qsTr("Join to our Instagram channel to get support and see the last changes.")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap

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
}
