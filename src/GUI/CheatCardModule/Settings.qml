//#
//# Copyright (C) 2021 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.15


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

            contentItem: ColumnLayout {
                implicitWidth: 0x0

                Text {
                    text: qsTr("Externality")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap
                    font.bold: true

                    Layout.fillWidth: true
                }

                SwitchDelegate {
                    id: control
                    text: qsTr("Change the main color of the application")
                    checked: false

                    Layout.fillWidth: true

                    contentItem: Text {
                        rightPadding: control.indicator.width + control.spacing
                        text: control.text
                        font: control.font
                        opacity: enabled ? 1.0 : 0.3
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                    }

                    indicator: Rectangle {
                        implicitWidth: 38
                        implicitHeight: 16
                        x: control.width - width - control.rightPadding
                        y: parent.height / 2 - height / 2
                        radius: 13
                        color: control.checked ? "#17a81a" : "transparent"
                        border.color: control.checked ? "#17a81a" : "#cccccc"

                        Rectangle {
                            x: control.checked ? parent.width - width : 0
                            y: -2
                            width: 20
                            height: 20
                            radius: 13
                            color: control.down ? "#cccccc" : "#ffffff"
                            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
                        }
                    }

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

            contentItem: ColumnLayout {
                implicitWidth: 0x0

                Text {
                    text: qsTr("Privacy")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap
                    font.bold: true

                    Layout.fillWidth: true
                }

                SwitchDelegate {
                    id: privacy
                    text: qsTr("Share name with seller")
                    checked: false

                    Layout.fillWidth: true

                    contentItem: Text {
                        rightPadding: privacy.indicator.width + privacy.spacing
                        text: privacy.text
                        font: privacy.font
                        opacity: enabled ? 1.0 : 0.3
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                    }

                    indicator: Rectangle {
                        implicitWidth: 38
                        implicitHeight: 16
                        x: privacy.width - width - privacy.rightPadding
                        y: parent.height / 2 - height / 2
                        radius: 13
                        color: privacy.checked ? "#17a81a" : "transparent"
                        border.color: privacy.checked ? "#17a81a" : "#cccccc"

                        Rectangle {
                            x: privacy.checked ? parent.width - width : 0
                            y: -2
                            width: 20
                            height: 20
                            radius: 13
                            color: privacy.down ? "#cccccc" : "#ffffff"
                            border.color: privacy.checked ? (privacy.down ? "#17a81a" : "#21be2b") : "#999999"
                        }
                    }

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

            contentItem: ColumnLayout {
                implicitWidth: 0x0

                Text {
                    text: qsTr("Camera")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    font.bold: true
                    wrapMode: Text.WordWrap

                    Layout.fillWidth: true
                }

                SwitchDelegate {
                    id: camera
                    text: qsTr("Select work camera")
                    checked: false

                    Layout.fillWidth: true

                    contentItem: Text {
                        rightPadding: camera.indicator.width + camera.spacing
                        text: camera.text
                        font: camera.font
                        opacity: enabled ? 1.0 : 0.3
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                    }

                    indicator: Rectangle {
                        implicitWidth: 38
                        implicitHeight: 16
                        x: camera.width - width - camera.rightPadding
                        y: parent.height / 2 - height / 2
                        radius: 13
                        color: camera.checked ? "#17a81a" : "transparent"
                        border.color: camera.checked ? "#17a81a" : "#cccccc"

                        Rectangle {
                            x: camera.checked ? parent.width - width : 0
                            y: -2
                            width: 20
                            height: 20
                            radius: 13
                            color: camera.down ? "#cccccc" : "#ffffff"
                            border.color: camera.checked ? (camera.down ? "#17a81a" : "#21be2b") : "#999999"
                        }
                    }

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

            contentItem: ColumnLayout {
                implicitWidth: 0x0

                Text {
                    text: qsTr("Developer settings")
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    wrapMode: Text.WordWrap
                    font.bold: true

                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    TextField { text: qsTr("Host") }
                    TextField { text: qsTr("Port") }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

    }
}
