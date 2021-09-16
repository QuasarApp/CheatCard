//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    visible: true
    height: 550
    width: 350

    property var model: mainModel
    property var user: (mainModel)? mainModel.currentUser: null

    header: ToolBar {
        position: ToolBar.Header
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: (userPanel.visible)? qsTr("<<") : qsTr("三")
                onClicked: (userPanel.visible)? userPanel.close() : userPanel.open()
                font.bold: true
            }

            Label {
                text: (user)?
                          qsTr("Hello ") + user.name +
                          ((user.fSaller)? qsTr(" (Saller mode)"):"")
                        : ""

                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                text: qsTr("⋮")
                font.bold: true
                font.pointSize: 14

                onClicked: mainMenu.popup()
            }
        }
    }

    Menu {
        id: mainMenu

        MenuItem {
            text: qsTr("Contact with developers")
        }

        MenuItem {
            text: qsTr("About")
        }
    }

    ColumnLayout {
        anchors.fill: parent

        SwipeView {
            id: view

            clip: true
            interactive: user && user.fSaller
            currentIndex: 0

            Layout.fillWidth: true
            Layout.fillHeight: true

            CardsListView {
                model: (mainWindow.model) ? mainWindow.model.ownCardsList: null
                visible: user && user.fSaller

                editable: user && user.fSaller
            }

            CardsListView {
                model: (mainWindow.model) ? mainWindow.model.cardsList: null
                editable: false
            }

        }

//        PageIndicator {
//            id: indicator
//            Layout.alignment: Qt.AlignHCenter
//            visible: user && user.fSaller
//            count: view.count
//            currentIndex: view.currentIndex
//            interactive: view.interactive
//        }
    }

    Drawer {
        id: userPanel
        y: header.height
//        width: 0.6 * mainWindow.width
        height: mainWindow.height

        contentItem: EditUserView {
            model: user
        }
    }

    Dialog {
        id: firstRun;
        visible: (model)? model.fFirst : false

        closePolicy: Popup.NoAutoClose

        height: mainWindow.height * 0.95
        width: mainWindow.width * 0.95
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2

        FirstRunPage {
            anchors.fill: parent
            model: mainModel
            onFinished: () => {
                            firstRun.close()
                        }
        }
    }

}
