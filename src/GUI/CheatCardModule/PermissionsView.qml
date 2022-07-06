//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "Style"

CPage {

    title: qsTr("Your Workers")

    id: root
    property var model: null
    waitModel: (model)? model.waitModel: null

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

                    UserView {
                        id: userView
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        height: implicitHeight
                        model: userObject
                        userDefaultAvatar: (root.model)?
                                               root.model.userDefaultAvatar(userID):
                                               ""
                        fCurrent: list.model && userID === list.model.currentUserId
                        onClick: {
                            if (list.model) {
                                list.model.currentUserId = userID
                            }
                        }
                    }

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                        icon.color: Material.accent
                        font.bold: true
                        font.pointSize: 14

                        onClicked: () => {
                                       if (userObject) {
                                           workerName.text = userObject.name
                                       }
                                       enternameDalog.currentRow = rowNumber
                                       enternameDalog.open()
                                   }
                    }

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                        icon.color: Material.accent
                        font.bold: true
                        font.pointSize: 14

                        onClicked: () => {
                                        list.model.removePermision(rowNumber)
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
        }
    }

    Dialog {
        id: enternameDalog

        property int currentRow: 0

        x: parent.width / 2 - enternameDalog.width / 2
        y: parent.height / 2 - enternameDalog.height / 2

        title: qsTr("Edit worker's name")
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (root.model) {
                root.model.setNewDescription(currentRow, workerName.text);
            }

        }

        contentItem: TextField {
            id: workerName
            placeholderText: qsTr("Enter your worker name.")
        }

        visible: false
    }
}
