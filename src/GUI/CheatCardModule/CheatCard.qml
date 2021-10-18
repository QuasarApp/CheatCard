//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import NotifyModule 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    height: 550
    width: 350

    property var model: mainModel
    property var user: (mainModel)? mainModel.currentUser: null

    Connections {
        target: model
        function onFreeItem(card, userId, freeItemsCount) {

            var activity = activityProcessor.newActivity("qrc:/CheatCardModule/FreeItemsView.qml",
                                                         card);
            activity.freeItems = freeItemsCount
            activity.userId = userId
        }

        function onConnectionWasEnd() {
            connectionStatus.close()

        }

        function onConnectionWasBegin() {
            connectionStatus.open()

        }
    }

    header: ToolBar {
        id: toolBar
        position: ToolBar.Header
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: (userPanel.visible || activityProcessor.depth > 1)? qsTr("<<") : qsTr("三")
                onClicked: () => {
                               if (activityProcessor.depth > 1) {
                                   activityProcessor.popItem();
                                   return;
                               }

                               if (userPanel.visible) {
                                   userPanel.close()

                                   if (mainModel) {
                                       mainModel.handleFirstDataSendet();
                                   }
                               } else {
                                   userPanel.open()
                               }
                           }
                font.bold: true
            }

            Label {
                text: (user)?
                          qsTr("Hello ") + user.name +
                          ((mainModel && mainModel.mode)? qsTr(" (Seller mode)"):"")
                        : ""

                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: menuButton
                text: qsTr("⋮")
                font.bold: true
                font.pointSize: 14

                onClicked: mainMenu.popup(this, menuButton.x, menuButton.height)
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

            onClicked:  () => {
                            activityProcessor.newActivityFromComponent(about, mainModel.getAboutModel());
                        }
        }
    }

    StackView {
        id: activityProcessor
        anchors.fill: parent

        function newActivityFromComponent(component, activityModel) {
            var activity = component.createObject(activityProcessor);
            if (activity === null) {
                // Error Handling
                console.error("Error creating Activity object. " + component.errorString());
                return;
            }

            if (activityModel) {
                activity.model = activityModel;
            }

            push(activity);

            return activity;
        }

        function newActivity(viewFile, activityModel) {

            if (!viewFile || !viewFile.length) {
                console.error("Failed to create activity. view object is invalid");
                return;
            }

            var component = Qt.createComponent(viewFile);

            if (component.status === Component.Ready) {
                var activity = component.createObject(activityProcessor);
                if (activity === null) {
                    // Error Handling
                    console.error("Error creating Activity object");
                    return;
                }

                if (activityModel) {
                    activity.model = activityModel;
                }

                push(activity);

                return activity;

            } else if (component.status === Component.Error) {
                // Error Handling
                console.log("Error loading component:", component.errorString());
            }


        }

        function popItem() {
            pop();
        }


        Component.onCompleted: () => {
                                   newActivity("qrc:/CheatCardModule/MainActivity.qml",
                                               mainModel);
                               }
    }

    Component {
        id: about
        About {}
    }

    Drawer {
        id: userPanel
        y: header.height
        height: mainWindow.height

        contentItem: EditUserView {
            model: mainModel
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

    Dialog {
        id: connectionStatus;

        closePolicy: Popup.NoAutoClose
        modal: true
        height: mainWindow.height * 0.95
        width: mainWindow.width * 0.95
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2

        ConnectionStatus {
            anchors.fill: parent
        }
    }

    NotificationServiceView {
        anchors.fill: parent;
    }
}
