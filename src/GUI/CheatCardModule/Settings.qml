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
    property var model: null

    contentItem: ColumnLayout {

        Frame {
            id: externality

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
                    font.pointSize: 12

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
                        font.pointSize: 11
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                    }

                }

                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal

                    padding: 1
                    topPadding: 1
                    bottomPadding: 1

                    contentItem: Rectangle {
                        implicitWidth: 1
                        implicitHeight: 1
                        color: "#c3c3c3"
                    }

                }

                Button {
                    id: selectColor
                    onClicked: () => {
                                   activityProcessor.newActivityFromComponent(settingdColor);

                               }
                    enabled: control.checked

                    text: qsTr("Select application color")
                    Layout.fillWidth: true

                    contentItem: Text {
                        text: selectColor.text
                        horizontalAlignment: Text.AlignVCenter
                        font.pointSize: 11
                        opacity: enabled ? 1.0 : 0.3
                        wrapMode: Text.WordWrap
                    }
                }

                Component {
                    id: settingdColor

                    ColorPicker {
                        id: colorPick
                        implicitHeight: 0x0
                        header: Label {
                            horizontalAlignment: Label.AlignHCenter
                            text: qsTr("Please choose a color")
                            font.bold: true
                        }

                        footer: DialogButtonBox {
                            onAccepted: () => {}

                            standardButtons: Dialog.Open
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
                    font.pointSize: 12

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
                        font.pointSize: 11
                        opacity: enabled ? 1.0 : 0.3
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
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
                    font.pointSize: 12
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
                        font.pointSize: 11
                        opacity: enabled ? 1.0 : 0.3
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
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
                    font.pointSize: 12

                    Layout.fillWidth: true
                }

                RowLayout {
                    TextField {
                        Layout.fillWidth: true
                        font.pointSize: 11
                        placeholderText: qsTr("Host")
                    }

                    TextField {
                        Layout.fillWidth: true
                        font.pointSize: 11
                        placeholderText: qsTr("Port")
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

    }
}
