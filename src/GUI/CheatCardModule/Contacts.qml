//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


import QtQuick 2.0

Page {
    id: root

    contentItem: ColumnLayout {

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: contactVariant

                anchors.fill: parent
                rows: 4
                columns: 1
                rowSpacing: 6
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10

                Rectangle {
//                    color: 'Red'
                    radius: 10
                    clip: false
                    Layout.margins: 10
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout {
                        anchors.fill: parent
                        ToolButton {
                            icon.source: "qrc:/images/private/resources/platformsIcon/slack-logo.svg"
                            icon.height: 100
                            icon.width: 100
                            onClicked: stack.pop()
                        }
                        Label {
                            text: "Telegram"
                            elide: Label.ElideRight
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }
                }

                Rectangle {
//                    color: 'Plum'
                    radius: 10
                    clip: false
                    Layout.margins: 10
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout {
                        anchors.fill: parent
                        ToolButton {
                            icon.height: 100
                            icon.width: 100
                            icon.source: "qrc:/images/private/resources/platformsIcon/telegram.svg"
                            onClicked: stack.pop()
                        }
                        Label {
                            text: "Slack"
                            elide: Label.ElideRight
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
