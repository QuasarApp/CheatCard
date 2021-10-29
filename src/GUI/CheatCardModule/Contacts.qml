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
                    color: "#777777"
                    antialiasing: true;
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
                            icon.height: 80
                            icon.width: 80
                            icon.color: "transparent"
                            onClicked: Qt.openUrlExternally("https://join.slack.com/t/slack-uvu5934/shared_invite/zt-xtxp3txy-z4E_8mFsabqSRIzy7AwJXg")
                        }

                        Text {
                            text: "Enjoy to our slack chanal for get support and see the last changes."
                            elide: Label.ElideRight
                            horizontalAlignment: Qt.AlignLeft
                            verticalAlignment: Qt.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.fillWidth: true
                        }
                    }
                }

                Rectangle {
                    color: "#777777"
                    antialiasing: true;
                    radius: 10
                    clip: false
                    Layout.margins: 10
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout {
                        anchors.fill: parent
                        ToolButton {
                            icon.height: 80
                            icon.width: 80
                            icon.color: "transparent"
                            icon.source: "qrc:/images/private/resources/platformsIcon/telegram.svg"
                            onClicked: Qt.openUrlExternally("https://t.me/joinchat/Msv_LWw4GI4zNTIy")
                        }

                        Text {
                            text: "Enjoy to our telegram chanal for get support and see the last changes."
                            elide: Label.ElideRight
                            horizontalAlignment: Qt.AlignLeft
                            verticalAlignment: Qt.AlignVCenter
                            wrapMode: Text.WordWrap

                            Layout.fillWidth: true
                        }
                    }
                }

                Item {
                    Layout.margins: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                }

                Item {
                    Layout.margins: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                }


            }
        }
    }
}
