//#
//# Copyright (C) 2021-2023 QuasarApp.
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

    id: root
    property var model: null
    property string userAvatar: ""
    property bool fCurrent: false;

    signal click(var mouse);

    background: Rectangle {
        color: (fCurrent)? Material.accent : Material.background
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                root.click(mouse)
            }
        }
    }

    contentItem:
        GridLayout {

        columns: 2
        rows: 1
        Item {
            id: imagePlace

            Layout.rowSpan: 1
            Layout.minimumWidth: 50
            Layout.minimumHeight: 50

            Image {
                id: userAvatarView
                asynchronous: true
                fillMode: Image.PreserveAspectFit

                source: (userAvatar.length)?
                            userAvatar:
                            "qrc:/images/private/resources/Logo/CheatCard_Logo1.png"
                anchors.fill: parent

            }
        }

        Label {
            id: nameEditor
            horizontalAlignment: Text.AlignLeft
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true

            text: (root.model && root.model.name)? root.model.name: qsTr("User")
        }
    }


}
