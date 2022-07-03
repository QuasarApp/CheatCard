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
    property var model: mainModel.permisionsModel

    contentItem:
        GridLayout {
        flow: (flowChecker.fHorisontal)?  GridLayout.LeftToRight :GridLayout.TopToBottom


        ExportUserKeyPage {
            fExport: true
            model: (root.model)? root.model.currentUserModel: null
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ListView {
            id: list
            model : root.model
            Layout.fillHeight: true
            Layout.fillWidth: true

            Component {
                id: delegateItem

                RowLayout {
                    UserView {
                        id: userView
                        width: list.width
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
                                   }
                    }

                    ToolButton {
                        icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                        icon.color: Material.accent
                        font.bold: true
                        font.pointSize: 14

                        onClicked: () => {
                                   }
                    }
                }
            }

            delegate: delegateItem

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: qsTr("Add new worker account")


                onClicked: {
                    enternameDalog.open()
                }
            }
        }
    }

    Dialog {
        id: enternameDalog
        x: parent.width / 2 - enternameDalog.width / 2
        y: parent.height / 2 - enternameDalog.height / 2

        title: qsTr("Create a new worker's account")
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (root.model)
                root.model.addNewPermision(workerName);
                workerName.text = "";
        }

        contentItem: TextField {
            id: workerName
            placeholderText: qsTr("Enter your worker name.")
        }

        visible: false
    }
}
