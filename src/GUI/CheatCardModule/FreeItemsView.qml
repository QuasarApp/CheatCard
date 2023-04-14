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
import QtGraphicalEffects 1.15

Frame {
    id: root
    property int freeItems: 0
    property var userId: null
    property bool fSeller: (mainModel)? mainModel.mode: false
    implicitHeight: 0x0
    property var model: null

    contentItem: ColumnLayout {

        Label {
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("Wooooh seems like someone has available %0 %1 !!!").
                arg(freeItems).
                arg(((root.model)? root.model.freeItem : ""))

            font.bold: true
            visible: text.length
            font.pointSize: 15
            wrapMode: Label.WordWrap
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
        }

        Image {
            id: cardLogoIamge
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: h
            Layout.preferredWidth: w

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: cardLogoIamge.width
                    height: cardLogoIamge.height
                    radius: Math.min(cardLogoIamge.height, cardLogoIamge.width)
                }
            }

            property int w:  Math.min(root.width, root.height) * 0.5
            property int h: Math.min(root.width, root.height) * 0.5

            source: "image://cards/seal:" +
                    ((root.model)? root.model.idbase64 : "0")

            opacity: 0

            Component.onCompleted:  {
                opacity = 1;
            }

            Behavior on w {
                NumberAnimation {
                    duration:  1000
                    easing.type: Easing.OutBounce
                }
            }

            Behavior on h {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        }

        RowLayout {

            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            visible: fSeller

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                icon.source: "qrc:/images/private/resources/Interface_icons/Less.svg"
                onClicked: ()=>{
                               giveFreeItems.value = giveFreeItems.from
                           }
            }

            SpinBox {
                id: giveFreeItems
                from: 1
                to: freeItems
            }

            ToolButton {
                icon.source: "qrc:/images/private/resources/Interface_icons/More.svg"

                onClicked: ()=>{
                               giveFreeItems.value = giveFreeItems.to
                           }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            Button {
                id: doNotGive
                visible: fSeller
                text: qsTr("Don`t give out")

                onClicked: ()=> {
                               activityProcessor.popItem();
                           }
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: doGive
                visible: fSeller
                text: qsTr("Give out")

                onClicked: ()=> {
                               if (mainModel && model) {
                                    mainModel.handleBonusGivOut(userId, model.idRaw, giveFreeItems.value);
                                    enabled = false
                                    activityProcessor.popItem();
                               }
                           }
            }
        }
    }
}
