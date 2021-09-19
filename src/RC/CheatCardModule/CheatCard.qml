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

ApplicationWindow {
    id: mainWindow
    visible: true
    height: 550
    width: 350

    property var model: mainModel
    property var user: (mainModel)? mainModel.currentUser: null

    Connections {
        target: model
        function onFreeItem(card, freeItemsCount) {

            var freeItems = Qt.createComponent("        FreeItemsView {
            freeItems: " + freeItemsCount + "
        }")

            activityProcessor.newActivityFromComponent(freeItems, card);

        }
    }

    header: ToolBar {
        position: ToolBar.Header
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: (userPanel.visible || activityProcessor.depth > 1)? qsTr("<<") : qsTr("三")
                onClicked: () => {
                               if (activityProcessor.depth > 1) {
                                   activityProcessor.pop();
                                   return;
                               }

                               if (userPanel.visible) {
                                   userPanel.close()
                               } else {
                                   userPanel.open()
                               }
                           }
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

    StackView {
        id: activityProcessor
        anchors.fill: parent

        function newActivityFromComponent(component, activityModel) {
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
            } else if (component.status === Component.Error) {
                // Error Handling
                console.log("Error loading component:", component.errorString());
            }
        }

        Component.onCompleted: () => {
                                   newActivity("qrc:/CheatCardModule/MainActivity.qml",
                                               mainModel);
                               }
    }

    Drawer {
        id: userPanel
        y: header.height
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
