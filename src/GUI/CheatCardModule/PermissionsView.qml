//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as Dialogs

import "Style"

CPage {

    toolBarTitle: qsTr("Your Workers")

    id: root
    property var model: null

    contentItem:
        ColumnLayout {
        spacing: 20

        Label {
            font.pointSize: 16
            font.bold: true
            color: "#999999"
            Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            verticalAlignment: Text.AlignVCenter | Text.AlignHCenter
            horizontalAlignment: Text.AlignVCenter | Text.AlignHCenter

            text: qsTr("If you want to add access to your cards, you can add workers.")
        }

        Label {
            color: "#999999"
            Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            verticalAlignment: Text.AlignVCenter
            text: qsTr("How to manage your workers:")
        }

        Label {
            color: "#999999"
            Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            text: qsTr("* Click the 'Scan QR code of your worker' button and scan a qr code from the sidebar of your worker's device.")
        }

        Label {
            color: "#999999"
            Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            text: qsTr("* Your worker will get access to all your cards")
        }

        Label {
            color: "#999999"
            Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            text: qsTr("* If you want to remove access of your cards, just remove the worker record on the bottom workers list menu.")
        }

        ListView {
            id: list
            model : root.model
            Layout.fillHeight: true
            Layout.fillWidth: true

            ScrollBar.vertical: ScrollBar {}

            Component {
                id: delegateItem

                RowLayout {
                    width: list.width
                    height: implicitHeight

                    RowLayout {

                                Item {
                                    Layout.fillWidth: true
                                }

                                Item {
                                    id: imagePlace

                                    Layout.rowSpan: 2
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true

                                    Image {

                                        id: userAvatarView
                                        asynchronous: true
                                        fillMode: Image.PreserveAspectFit

                                        source: defaultAvatar
                                        anchors.fill: parent

                                    }

                                }

                                Label {
                                    id: nameEditor
                                    text: permisionDescription
                                }

                                Item {
                                    Layout.fillWidth: true
                                }
                            }

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                        icon.color: Material.accent
                        font.bold: true
                        font.pointSize: 14

                        onClicked: () => {
                                       workerName.text = permisionDescription
                                       enternameDalog.editableKey = permisionKey
                                       enternameDalog.open()
                                   }
                    }

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                        icon.color: Material.accent
                        font.bold: true
                        font.pointSize: 14

                        onClicked: () => {
                                       list.model.removePermision(permisionKey)
                                   }
                    }
                }
            }

            delegate: delegateItem

        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter

            ToolButton {
                Layout.alignment: Qt.AlignHCenter

                icon.source: "qrc:/images/private/resources/Interface_icons/qrscaner.svg"
                icon.height: 60
                icon.width: 60

                icon.color: Material.accent
                font.bold: true
                font.pointSize: 14

                Component {
                    id: scaner
                    QrScanner {
                        id:qrScaner
                        onCaptured: (data) => {
                                        root.model.addNewPermision(data);
                                        activityProcessor.popItem();

                                    }

                        onVisibleChanged: {
                            if(!visible)
                                destroy()
                        }
                    }
                }

                onClicked: {
                    activityProcessor.newActivityFromComponent(scaner);
                }
            }

            ToolButton {
                Layout.alignment: Qt.AlignHCenter

                icon.source: "qrc:/images/private/resources/Interface_icons/background_image.svg"
                icon.height: 50
                icon.width: 60

                icon.color: Material.accent
                font.bold: true
                font.pointSize: 14

                Dialogs.FileDialog {
                    id: fromFile
                    folder: shortcuts.documents

                    selectFolder: false
                    selectMultiple: false
                    selectExisting: true
                    title: qsTr("Select image with qr code of your worker")
                    onSelectionAccepted: {
                        root.model && root.model.addNewPermisionFromImage(fileUrl)
                    }
                }

                onClicked: {
                    fromFile.open()
                }
            }
        }
    }

    Dialog {
        id: enternameDalog

        property var editableKey: null

        x: parent.width / 2 - enternameDalog.width / 2
        y: parent.height / 2 - enternameDalog.height / 2

        title: qsTr("Edit worker's name")
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (root.model) {
                root.model.setNewDescription(editableKey, workerName.text);
            }

        }

        contentItem: TextField {
            id: workerName
            placeholderText: qsTr("Enter your worker name.")
        }

        visible: false
    }
}
