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

Page {

    id: root
    property var model: mainModel.usersListModel

    contentItem: ListView {
        id: list
        model : root.model


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
    }
}
