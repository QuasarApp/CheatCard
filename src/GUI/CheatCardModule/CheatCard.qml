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
import NotifyModule 1.0

ApplicationWindow {
    id: mainWindow
    visible: true

    //  Vertical mode
    height: 640
    width: 350

    // Horisontal mode
//    height: 350
//    width: 640


    onClosing: {
        // this is bad solution. but it is works fine.
        // we handle close event that emit when user click back button and use own back button if it is needed.
        if (backButton.fBack && backButton.enabled && Qt.platform.os === "android") {
            backButton.clicked();
            close.accepted = false
        }
    }

    readonly property string defaultpPimaryColor: "#ff6b01"
    property var model: mainModel
    property var user: (mainModel)? mainModel.currentUser: null
    Material.primary: config.getStrValue("colorTheme", defaultpPimaryColor)
    Material.accent: Material.Orange


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

    Connections {
        target: config
        function onValueStrChanged(key, value) {
            if (key === "colorTheme") {
                if (!value)
                    value = defaultpPimaryColor
                mainWindow.Material.primary = value;
            }
        }
    }

    header: ToolBar {
        id: toolBar

        Material.foreground: "#ffffff"

        position: ToolBar.Header
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: backButton
                property bool fBack: (userPanel.visible || activityProcessor.depth > 1)
                icon.source: (fBack)? "qrc:/images/private/resources/Interface_icons/Close_sidebar.svg":
                                      "qrc:/images/private/resources/Interface_icons/Open_sidebar.svg"

                onClicked: () => {
                               if (!enabled)
                                   return;

                               if (activityProcessor.depth == 2) {
                                   if (mainModel) {
                                       mainModel.handleFirstDataSendet();
                                   }
                               }

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

                enabled: !firstRun.visible

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
                icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                font.bold: true
                font.pointSize: 14
                enabled: !firstRun.visible

                onClicked: mainMenu.popup(this, menuButton.x, menuButton.height)
            }
        }
    }

    Menu {
        id: mainMenu

        MenuItem {
            text: qsTr("Contact with developers")
            visible: (mainModel)? mainModel.mode: false

            height: visible ? implicitHeight : 0

            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/Contacts.qml");
                        }
        }

        MenuItem {
            text: qsTr("About")

            onClicked:  () => {
                            activityProcessor.newActivityFromComponent(about, mainModel.getAboutModel());
                        }
        }

        MenuItem {
            text: qsTr("Help")

            onClicked:  () => {

                            if (mainModel.mode) {
                                activityProcessor.newActivity("qrc:CheatCardModule/PageHelpSeller.qml");
                            } else {
                                activityProcessor.newActivity("qrc:CheatCardModule/PageHelpVisitor.qml");
                            }


                        }
        }
	
        MenuItem {
            text: qsTr("Settings")

            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/Settings.qml");
                        }
        }

        MenuItem {
            text: qsTr("Recovery cards")

            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/RecoveryPage.qml");
                        }
        }

        MenuItem {
            text: qsTr("Export your key")

            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/ExportUserKeyPage.qml");
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

    Component {
        id: settings
        Settings {}
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
