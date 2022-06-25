//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import com.scythestudio.scodes 1.0

Page {

    id: root
    property var model: mainModel.usersListModel

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

            }

            delegate: delegateItem

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: qsTr("Import new user")
            }

        }
    }

}
