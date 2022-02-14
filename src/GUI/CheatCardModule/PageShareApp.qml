//#
//# Copyright (C) 2021 - 2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


Page {
    id: root

    implicitHeight: 0x0

    contentItem: GridLayout {
        id: rootGrid

        flow: (alignroot1.fHorisontal | alignroot2.fHorisontal)? GridLayout.LeftToRight : GridLayout.TopToBottom

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            SwipeView {
                id: view

                currentIndex: 0
                Layout.fillWidth: true
                Layout.fillHeight: true

                Page {
                    id: firstPage

                    contentItem:
                    ColumnLayout {

                        Image {
                            id: imgLogo1
                            fillMode: Image.PreserveAspectFit
                            Layout.preferredWidth:  Math.min(root.width * 0.4, root.height * 0.4)
                            Layout.preferredHeight: width * 0.4
                            Layout.alignment: Qt.AlignHCenter
                            source: "qrc:/images/private/resources/Interface_icons/Google_play_logo.png"
                        }

                        Label {
                            text: qsTr("To get a cheat card, scan the QR code and follow the link.")
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignTop
                            wrapMode: Text.WordWrap
                            font.pointSize: 14

                            Layout.fillWidth: true
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    Image {
                        id: imgQr1
                        fillMode: Image.PreserveAspectFit
                        Layout.preferredHeight: Math.min(root.width * 0.8, root.height * 0.8)
                        Layout.preferredWidth:  Math.min(root.width * 0.8, root.height * 0.8)
                        Layout.alignment: Qt.AlignHCenter
                        source: "qrc:/images/private/resources/Interface_icons/qr-code_android_link.png"
                        layer.enabled: true
                        layer.effect: ShaderColorOverlay {
                            color: Material.primary
                            fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
                        }
                    }

                    Item {
                        id: alignroot1
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        property bool fHorisontal: rootGrid.height < rootGrid.width
                    }

                }

                Page {
                    id: secondPage

                    contentItem:
                    ColumnLayout {
                        Image {
                            id: imgLogo2
                            fillMode: Image.PreserveAspectFit
                            Layout.preferredWidth:  Math.min(root.width * 0.4, root.height * 0.4)
                            Layout.preferredHeight: width * 0.4
                            Layout.alignment: Qt.AlignHCenter
                            source: "qrc:/images/private/resources/Interface_icons/AppStore.png"
                        }

                        Label {
                            text: qsTr("To get a cheat card, scan the QR code and follow the link.")
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignTop
                            wrapMode: Text.WordWrap
                            font.pointSize: 14

                            Layout.fillWidth: true
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    Image {
                        id: imgQr2
                        fillMode: Image.PreserveAspectFit
                        Layout.preferredHeight: Math.min(root.width * 0.8, root.height * 0.8)
                        Layout.preferredWidth:  Math.min(root.width * 0.8, root.height * 0.8)
                        Layout.alignment: Qt.AlignHCenter
                        source: "qrc:/images/private/resources/Interface_icons/qr-code_apple_link.png"
                        layer.enabled: true
                        layer.effect: ShaderColorOverlay {
                            color: Material.primary
                            fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
                        }
                    }

                    Item {
                        id: alignroot2
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        property bool fHorisontal: rootGrid.height < rootGrid.width
                    }
                }

            }

            PageIndicator {
                id: indicator

                count: view.count
                currentIndex: view.currentIndex

                Layout.alignment: Qt.AlignHCenter

            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                ToolButton {
                    icon.source: "qrc:/images/private/resources/Interface_icons/android_icon_btn.svg"
                    icon.height: 30
                    icon.width: 30
                    icon.color: "transparent"
                    Layout.alignment: Qt.AlignRight
                    enabled: view.currentIndex
                    onClicked: () => {
                                   view.currentIndex--;
                               }
                }

                ToolButton {
                    id: nextButton
                    icon.source: "qrc:/images/private/resources/Interface_icons/apple_icon_btn.svg"
                    icon.height: 30
                    icon.width: 30
                    icon.color: "transparent"
                    Layout.alignment: Qt.AlignLeft
                    enabled: view.currentIndex != 1
                    onClicked: () => {
                                   view.currentIndex++;
                               }
                }
            }

        }

    }

}
