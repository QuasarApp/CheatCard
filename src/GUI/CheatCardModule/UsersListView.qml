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
import com.scythestudio.scodes 1.0
import "Style"
CPage {

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

            delegate: RowLayout {

                width: list.width
                height: implicitHeight

                UserView {
                    id: userView
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ListView.onRemove: SequentialAnimation {

                        PropertyAction {
                            target: userView
                            property: "ListView.delayRemove"
                            value: true
                        }

                        NumberAnimation {
                            target: userView
                            property: "x"
                            to: -userView.width
                            duration: 200
                            easing.type: Easing.InOutQuad
                        }

                        PropertyAction {
                            target: userView;
                            property: "ListView.delayRemove"
                            value: false
                        }
                    }
                    model: userObject
                    userAvatar: (root.model)?
                                    defaultAvatar:
                                    ""
                    fCurrent: userObject === list.model.currentUserModel
                    onClick: {
                        if (list.model) {
                            list.model.currentUserKey = model.key
                        }
                    }
                }

                ToolButton {
                    icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                    icon.color: Material.accent
                    font.bold: true
                    font.pointSize: 14
                    visible: list.count > 1
                    onClicked: () => {
                                   if (list.model) {
                                       list.model.removeUser(userObject.key)
                                   }
                               }
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: qsTr("Import new user")

                Component {
                    id: scanerCompanent
                    QrScanner {
                        id:qrScaner

                        onCaptured: (data) => {
                                        const model = mainModel.exportImportModel

                                        if (model)
                                        model.handleDecodeFinished(data);

                                        activityProcessor.popItem();

                                    }

                        onVisibleChanged: {
                            if(!visible)
                                destroy()
                        }
                    }
                }

                onClicked: {
                    activityProcessor.newActivityFromComponent(scanerCompanent);
                }
            }

        }
    }

}
