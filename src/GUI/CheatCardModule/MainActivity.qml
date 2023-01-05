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
import QtQuick.Controls.Material.impl 2.15

ColumnLayout {
    id: root

    property var model: null
    property alias mainList: mainListView

    CardsListView {
        id: mainListView
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: (root.model) ? root.model.cardsList: null


        editable: Boolean(root.model && root.model.mode)
        visible: !firstRun.visible

        Row {

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 10
            visible: mainListView.model.sourceModel.rowCount() > 4

            onVisibleChanged: {
                if (!visible)
                    mainListView.model.setFilterFixedString("")
            }

            TextField {
                id: filterLine
                placeholderText: qsTr("Search a card");
                opacity: 0;
                visible: opacity
                onTextChanged: {
                    mainListView.model.setFilterFixedString(text)
                }

                onVisibleChanged: {
                    if (!visible)
                        mainListView.model.setFilterFixedString("")
                }


                Behavior on opacity {
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.InQuad
                    }
                }
            }


            ToolButton {
                id: lambdsSearchButton
                icon.source: "qrc:/images/private/resources/Interface_icons/search.svg"
                font.bold: true
                font.pointSize: 14
                icon.color: Material.accent

                opacity: 0.6

                onClicked: {
                    filterLine.opacity = !filterLine.opacity
                }
            }
        }


        ToolButton {
            id: lambdsButtonQrCode
            icon.source: "qrc:/images/private/resources/Interface_icons/qrcode.svg"
            font.bold: true
            font.pointSize: 14
            visible: !mainListView.editable
            width: 70
            height: 70

            icon.height: 40
            icon.width: 40

            background:
                Rectangle {
                    color: "#00000000"
                    border.width: 2
                    border.color: Material.accent
                    radius: Math.min(width, height) / 2
                    opacity: 0.3

                    Ripple {
                        implicitWidth: lambdsButtonQrCode.Material.touchTarget
                        implicitHeight: lambdsButtonQrCode.Material.touchTarget

                        readonly property bool square: lambdsButtonQrCode.contentItem.width <= lambdsButtonQrCode.contentItem.height

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        clip: !square
                        width: square ? parent.height / 2 : parent.width
                        height: square ? parent.height / 2 : parent.height
                        pressed: lambdsButtonQrCode.pressed
                        anchor: lambdsButtonQrCode
                        active: lambdsButtonQrCode.enabled && (lambdsButtonQrCode.down || lambdsButtonQrCode.visualFocus || lambdsButtonQrCode.hovered)
                        color: lambdsButtonQrCode.Material.rippleColor
                    }
                }


            icon.color: Material.accent
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.bottomMargin: 20

            opacity: 0.6

            onClicked: {
                backButton.onClicked()
            }
        }
    }

    FirstRunPage {
        id: firstRun
        visible: (model)? model.fFirst : false

        Layout.fillHeight: true
        Layout.fillWidth: true
        model: mainModel

    }
}

