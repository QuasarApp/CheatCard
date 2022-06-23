import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "Style"

CPage {
    id: customUiPage
    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent
            spacing: 20
            Item {
                Layout.fillHeight: true
            }

            Pane {
                id: externality

                Layout.margins: 10
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true

                contentItem: ColumnLayout {
                    implicitWidth: 0x0

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
                        checked: false
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

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
