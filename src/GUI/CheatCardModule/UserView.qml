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
    property var model: null
    property string userDefaultAvatar: ""
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

    contentItem: RowLayout {

            Item {
                Layout.fillWidth: true
            }

            Item {
                id: imagePlace

                Layout.rowSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {

                    id: userAvatar
                    fillMode: Image.PreserveAspectFit

                    source: (userDefaultAvatar.length)?
                                userDefaultAvatar:
                                ":/images/private/resources/Logo/CheatCard_Logo1.png"
                    anchors.fill: parent

                }

            }

            GridLayout {

                columns: 2
                rows: 2

                Label {
                    text: qsTr("Id:");
                }

                Label {

                    text: (root.model)? root.model.userId: ""
                    horizontalAlignment: Label.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter

                }

                Label {
                    text: qsTr("Name:");

                }

                Label {
                    id: nameEditor
                    text: (root.model)? root.model.name: ""
                }

            }

            Item {
                Layout.fillWidth: true
            }
        }
}
