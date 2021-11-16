//#
//# Copyright (C) 2021 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

import QtQuick.Controls.Material 2.15


Page {
    id: rootSettins

    contentItem: Flickable {
        id: flickable
        ScrollBar.vertical: ScrollBar {}
        contentHeight: listSettings.height
        clip: true


        ColumnLayout {
            id: listSettings
            anchors.fill: parent

            Pane {
                id: externality

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Appearance")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Text.WordWrap
                        font.bold: true
                        font.pointSize: 12

                        Layout.fillWidth: true
                    }

                    RowLayout {

                        Text {
                            text: qsTr("Select application color")
                            horizontalAlignment: Text.AlignVCenter
                            font.pointSize: 11
                            opacity: enabled ? 1.0 : 0.3
                            wrapMode: Text.WordWrap

                            Layout.fillWidth: true
                        }

                        Rectangle {
                            id: colorView
                            width: 25
                            height: 25
                            radius: 5
                            color: Material.primary
                        }

                        Button {
                            id: btnSelectColor
                            text: qsTr("Select")
                            font.pointSize: 9

                            onClicked: () => {
                                           activityProcessor.newActivityFromComponent(settingdColor);
                                       }
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
                                onAccepted: () => {
                                                colorView.color = colorPick.color
                                                config.colorTheme = colorPick.color
                                                config.setValue("colorTheme", colorPick.color)

                                                activityProcessor.popItem();

                                            }

                                standardButtons: Dialog.Open
                            }
                        }
                    }
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
                    color: Material.primary
                }

            }

            Pane {

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
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
                        padding: 0

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

            ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal

                padding: 1
                topPadding: 1
                bottomPadding: 1

                contentItem: Rectangle {
                    implicitWidth: 1
                    implicitHeight: 1
                    color: Material.primary
                }

            }

            Pane {
                id: cameraPage

                property var devicesList: QtMultimedia.availableCameras
                property var comboBoxModel: []

                onDevicesListChanged: {
                    cameraPage.comboBoxModel = []
                    cameraPage.devicesList.forEach((item)=>{cameraPage.comboBoxModel += item.displayName})
                }

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Camera")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        font.bold: true
                        font.pointSize: 12
                        wrapMode: Text.WordWrap

                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Text {
                            id: name
                            text: qsTr("Select work camera")
                            font.pointSize: 11
                            opacity: enabled ? 1.0 : 0.3
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        ComboBox {
                            id: selectCamera
                            editable: true

                            Layout.fillWidth: true

                            model: cameraPage.comboBoxModel
                        }
                    }
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
                    color: Material.primary
                }

            }

            Pane {

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Developer settings")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Text.WordWrap
                        font.bold: true
                        font.pointSize: 12

                        Layout.fillWidth: true
                    }

                    SwitchDelegate {
                        id: unlock
                        text: qsTr("Use custom server")
                        checked: false
                        padding: 0

                        Layout.fillWidth: true

                        contentItem: Text {
                            text: unlock.text
                            font.pointSize: 11
                            opacity: enabled ? 1.0 : 0.3
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                        }
                    }

                    RowLayout {
                        TextField {
                            enabled: unlock.checked
                            Layout.fillWidth: true
                            font.pointSize: 11
                            placeholderText: qsTr("Host")
                        }

                        TextField {
                            enabled: unlock.checked
                            Layout.fillWidth: true
                            font.pointSize: 11
                            placeholderText: qsTr("Port")
                        }
                    }
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
                    color: Material.primary
                }

            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

            }

        }
    }
}
