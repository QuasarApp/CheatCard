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
import NotifyModule 1.0
import SettingsKeys 1.0

import "Style"

ApplicationWindow {
    id: mainWindow
    visible: true

    font.pointSize: 13

    //  Vertical mode
    height: 800
    width: 600

    SettingsKeys {
        id: settingsKeys
    }

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

    property var model: mainModel
    property var user: (model && model.usersListModel)?
                           model.usersListModel.currentUserModel: null

    Material.primary: config.getStrValue(settingsKeys.COLOR_THEME)
    Material.accent: Material.primary
    Material.theme: (config.getValue(settingsKeys.DARK_THEME))? Material.Dark : Material.Light

    Connections {
        target: config
        function onValueStrChanged(key, value) {
            if (key === settingsKeys.COLOR_THEME) {
                mainWindow.Material.primary = value;
            }
        }

        function onValueChanged(key, value) {
            if (key === settingsKeys.DARK_THEME) {
                mainWindow.Material.theme = (value)? Material.Dark : Material.Light
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

                               if (activityProcessor.depth > 1) {
                                   activityProcessor.popItem();
                                   return;
                               }

                               if (userPanel.visible) {
                                   userPanel.close()
                               } else {
                                   userPanel.open()
                               }
                           }

                font.bold: true

                enabled: !((model)? model.fFirst : false) || fBack

            }

            ToolButton {
                id: switchUserButton
                icon.source: "qrc:/images/private/resources/Interface_icons/user.svg"
                font.bold: true
                font.pointSize: 14
                visible: mainModel && mainModel.usersListModel.usersCount > 1
                onClicked: {
                    activityProcessor.newActivityFromComponent(pageUsers);
                }
            }

            Label {
                text: activityProcessor.cuurentActivityTitle

                font.pointSize: 14
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: notificationsIcon
                icon.source: "qrc:/images/private/resources/Interface_icons/notification.svg"
                font.bold: true
                font.pointSize: 12
                visible: notificationService.notificationsCount
                onClicked: {
                    notificationService.showHistory()
                }
            }

            NetIndicatorModel {}

            ToolButton {
                id: menuButton
                icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                font.bold: true
                font.pointSize: 14
                enabled: !((model)? model.fFirst : false)

                onClicked: mainMenu.popup(this, menuButton.x, menuButton.height)
            }
        }
    }

    CMenu {
        id: mainMenu

        MenuItem {
            visible: (mainModel)? mainModel.mode: false
            height: (visible)? ganeralMenuItem.height: 0
            enabled: !mainActivity.mainList.hasEdit
            text: qsTr("Create a card")
            icon.source: "qrc:/images/private/resources/Interface_icons/CreateCard.svg"
            onClicked:  () => {
                            mainActivity.mainList.createCard()
                        }
        }

        MenuItem {
            height: (visible)? ganeralMenuItem.height: 0

            text: qsTr("Contact with developers")
            icon.source: "qrc:/images/private/resources/Interface_icons/contact_developers.svg"
            onClicked:  () => {
                            activityProcessor.newActivityFromComponent(pageContactdev);
                        }
        }

        MenuItem {
            id: ganeralMenuItem
            text: qsTr("About")
            icon.source: "qrc:/images/private/resources/Interface_icons/help.svg"
            onClicked:  () => {
                            activityProcessor.newActivityFromComponent(about, mainModel.getAboutModel());
                        }

        }

        MenuItem {
            text: qsTr("Share application")
            icon.source: "qrc:/images/private/resources/Interface_icons/sharing_icon.svg"
            onClicked:  () => {
                            activityProcessor.newActivityFromComponent(shareApp);
                        }

        }

        MenuItem {
            text: qsTr("Help")
            icon.source: "qrc:/images/private/resources/Interface_icons/about.svg"
            onClicked:  () => {

                            if (mainModel.mode) {
                                activityProcessor.newActivityFromComponent(pageSeller);
                            } else {
                                activityProcessor.newActivityFromComponent(pageVisitor);
                            }


                        }
        }

        MenuItem {
            text: qsTr("Settings")
            icon.source: "qrc:/images/private/resources/Interface_icons/settings.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(settings);
                       }

        }

        MenuItem {
            text: qsTr("Import Account")
            icon.source: "qrc:/images/private/resources/Interface_icons/key_pull.svg"

            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/ImportUserKeyPage.qml",
                                                          mainModel.exportImportModel);
                        }
        }

        MenuItem {
            text: qsTr("Export Account")
            icon.source: "qrc:/images/private/resources/Interface_icons/key_push.svg"
            onClicked:  () => {
                            activityProcessor.newActivity("qrc:/CheatCardModule/ExportUserKeyPage.qml",
                                                          user);
                        }
        }
    }

    ActivityProcessor {
        id: activityProcessor
        model: (mainWindow.model)? mainWindow.model.activityProcessorModel: null
        anchors.fill: parent

        initialItem: MainActivity {
            id: mainActivity
            model: mainModel
            user: mainWindow.user
        }
    }

    Component {
        id: pageUsers
        UsersListView {}
    }

    Component {
        id: pageContactdev
        Contacts {}
    }

    Component {
        id: about
        About {}
    }

    Component {
        id: shareApp
        PageShareApp {}
    }

    Component {
        id: pageSeller
        PageHelpSeller {}
    }

    Component {
        id: pageVisitor
        PageHelpVisitor {}
    }

    Component {
        id: settings
        Settings {}
    }

    Drawer {
        id: userPanel
        y: header.height
        height: mainWindow.height - toolBar.height
        contentItem: EditUserView {
            model: mainModel
            maximuWidth:  mainWindow.width - leftPadding - rightPadding
        }
    }

    NotificationServiceView {
        anchors.fill: parent;
    }

    WaitView {
        model: mainModel.waitModel
    }

    Item {
        id: flowChecker

        property bool fHorisontal: mainWindow.height < mainWindow.width
    }
}
