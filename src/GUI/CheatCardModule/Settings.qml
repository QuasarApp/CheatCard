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
            width: flickable.width

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
                        wrapMode: Label.WordWrap
                        font.bold: true

                        Layout.fillWidth: true
                    }

                    RowLayout {

                        Label {
                            text: qsTr("Select application color")
                            horizontalAlignment: Label.AlignVCenter
                            opacity: enabled ? 1.0 : 0.3
                            wrapMode: Label.WordWrap

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

                            onClicked: () => {
                                           activityProcessor.newActivityFromComponent(settingdColor);
                                       }
                        }

                    }

                    SwitchDelegate {
                        id: darkTheme
                        text: qsTr("Dark Theme")
                        checked: config.getValue("darkTheme", false)
                        padding: 0

                        onCheckedChanged: () => {
                                              config.setValue("darkTheme", darkTheme.checked)
                                          }

                        Layout.fillWidth: true

                        contentItem: Label {
                            rightPadding: darkTheme.indicator.width + darkTheme.spacing
                            text: darkTheme.text
                            opacity: enabled ? 1.0 : 0.3
                            elide: Label.ElideRight
                            verticalAlignment: Label.AlignVCenter
                            wrapMode: Label.WordWrap
                        }

                    }

                    RowLayout {

                        Label {
                            text: qsTr("Language")
                            horizontalAlignment: Label.AlignVCenter
                            opacity: enabled ? 1.0 : 0.3
                            wrapMode: Label.WordWrap

                            Layout.fillWidth: true
                        }

                        ComboBox {
                            id: langSelector
                            model: (mainModel)? mainModel.langModel: null
                            displayText: (model)? model.getCurrentLanguage(): "";
                            onActivated: {
                                if (model) {
                                    model.selectLanguagge(currentText, this)
                                }
                            }
                        }
                    }

                    Component {
                        id: settingdColor

                        ColorPicker {
                            id: colorPick
                            currentlyColor: Material.primary
                            implicitHeight: 0x0
                            title: qsTr("Please choose a color")

                            footer: DialogButtonBox {
                                onAccepted: () => {
                                                colorView.color = colorPick.color
                                                config.setStrValue("colorTheme", colorPick.color)

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
                    color: "#c3c3c3c3"
                }

            }

            Pane {
                id: privacySettings

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Privacy")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap
                        font.bold: true

                        Layout.fillWidth: true
                    }

                    SwitchDelegate {
                        id: privacy
                        text: qsTr("Share name with seller")
                        checked: config.getValue("shareName", true)
                        padding: 0

                        onCheckedChanged: () => {
                                              config.setValue("shareName", privacy.checked)
                                          }

                        Layout.fillWidth: true

                        contentItem: Label {
                            rightPadding: privacy.indicator.width + privacy.spacing
                            text: privacy.text
                            opacity: enabled ? 1.0 : 0.3
                            elide: Label.ElideRight
                            verticalAlignment: Label.AlignVCenter
                            wrapMode: Label.WordWrap
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
                    color: "#c3c3c3c3"
                }

            }

            Pane {
                id: cameraPage

                property var devicesList: QtMultimedia.availableCameras

                onDevicesListChanged: {
                    let comboBoxModel = [];
                    cameraPage.devicesList.forEach((item) =>
                                                   {
                                                       if (!comboBoxModel.includes(item))
                                                           comboBoxModel.push(item.deviceId)
                                                   })

                    selectCamera.model = comboBoxModel;
                }

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Work rules")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        font.bold: true
                        wrapMode: Label.WordWrap

                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Label {
                            id: name
                            text: qsTr("Select work camera")
                            opacity: enabled ? 1.0 : 0.3
                            verticalAlignment: Label.AlignVCenter
                            wrapMode: Label.WordWrap
                            Layout.fillWidth: true
                        }

                        ComboBox {
                            id: selectCamera
                            enabled: model.length

                            displayText: config.getStrValue("cameraDevice",
                                                            (model.length? model[0]: ""))


                            onActivated: (index) => {
                                const newId = model[index];
                                config.setStrValue("cameraDevice", newId)
                                displayText = newId;
                            }

                            Layout.fillWidth: true
                        }

                    }

                    RowLayout {
                        Label {
                            text: qsTr("Select API version")
                            opacity: enabled ? 1.0 : 0.3
                            verticalAlignment: Label.AlignVCenter
                            wrapMode: Label.WordWrap
                            Layout.fillWidth: true
                        }

                        ComboBox {
                            enabled: model.length
                            model: [1,2]
                            displayText: config.getStrValue("APIVersion", 2)


                            onActivated: (index) => {
                                const newId = model[index];
                                config.setStrValue("APIVersion", newId)
                                displayText = newId;
                            }

                            Layout.fillWidth: true
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
                    color: "#c3c3c3c3"
                }

            }

            Pane {
                id: devSettings

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Developer settings")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap
                        font.bold: true

                        Layout.fillWidth: true
                    }

                    Button {
                        text: qsTr("Check database location")
                        Layout.fillWidth: true

                        onClicked: {
                            config.showDataBaseLocation();
                        }
                    }

                    Button {
                        text: qsTr("Export a database")
                        Layout.fillWidth: true

                        onClicked: {
                            config.exportDataBase();
                        }
                    }

                    Button {
                        text: qsTr("Diagnostic the application")
                        Layout.fillWidth: true;

                        onClicked: {
                            activityProcessor.newActivity("qrc:/CheatCardModule/DiagnoticPage.qml",
                                                           mainModel.doctorModel);
                        }
                    }

                    SwitchDelegate {
                        id: unlock
                        text: qsTr("Use custom server")
                        checked: config.getValue("devSettingEnable", false)
                        padding: 0

                        onCheckedChanged: () => {
                                              config.setValue("devSettingEnable", unlock.checked)
                                          }

                        Layout.fillWidth: true

                        contentItem: Label {
                            text: unlock.text
                            opacity: enabled ? 1.0 : 0.3
                            verticalAlignment: Label.AlignVCenter
                            wrapMode: Label.WordWrap
                        }
                    }

                    RowLayout {
                        TextField {
                            id: host
                            opacity: unlock.checked
                            visible: Boolean(opacity)
                            Layout.fillWidth: true
                            placeholderText: qsTr("Host")
                            text: config.getStrValue("host", "")

                            onEditingFinished: () => {
                                                   config.setStrValue("host", host.displayText)
                                               }

                            Behavior on opacity {
                                NumberAnimation {
                                    duration: 550
                                    easing.type: Easing.OutCirc
                                }
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
                    color: "#c3c3c3c3"
                }

            }

            Pane {
                id: resetSettings

                Layout.margins: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

                    Label {
                        text: qsTr("Default settings")
                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Label.WordWrap
                        font.bold: true

                        Layout.fillWidth: true
                    }

                    Button {
                        id: btnReset
                        text: qsTr("Reset all settings")
                        Layout.alignment: Qt.AlignHCenter

                        onClicked: () => {
                                       config.resetToDefault()
                                       activityProcessor.popItem();

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
                    color: "#c3c3c3c3"
                }

            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

            }

        }
    }
}
