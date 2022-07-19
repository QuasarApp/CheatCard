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
    property bool editable: true
    property bool hasEdit: false
    property alias cardCount: list.count

    signal finished()

    function createCard() {
        activityProcessor.newActivity("qrc:/CheatCardModule/CreateCardView.qml",
                                      mainModel.createCardModel);
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {

            id: list
            model: root.model
            currentIndex: count - 1;
            interactive: !hasEdit
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter
            implicitHeight: 0x0
            implicitWidth: 0x0

            snapMode: ListView.NoSnap
            boundsBehavior:Flickable.StopAtBounds
            preferredHighlightBegin: (orientation == ListView.Vertical)?
                                         list.height / 2 - itemHeight / 2 :
                                         list.width / 2 - itemWidth / 2
            preferredHighlightEnd: preferredHighlightBegin + ((orientation == ListView.Vertical)?
                                                                  itemHeight :
                                                                  itemWidth )

            highlightRangeMode: ListView.StrictlyEnforceRange
            ScrollBar.vertical: ScrollBar {
                id: vScrollBar
                enabled: !hasEdit && list.orientation == ListView.Vertical
            }

            ScrollBar.horizontal: ScrollBar {
                id: hScrollBar
                enabled: !hasEdit && list.orientation == ListView.Horizontal
            }

            orientation: (root.width < root.height)? ListView.Vertical : ListView.Horizontal

            Pane {
                padding: 20
                anchors.fill: parent
                visible: !list.count

                contentItem: ColumnLayout {

                    spacing: 20

                    Item {
                        Layout.fillHeight: true
                    }

                    Label {

                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillWidth: true

                        text: qsTr("Don't have any cards yet? ")
                        font.pointSize: 20
                        font.bold: true
                        color: "#999999"
                        wrapMode: Label.WordWrap
                        horizontalAlignment: Text.AlignJustify
                        verticalAlignment: Text.AlignVCenter

                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter |  Qt.AlignVCenter
                        Layout.fillWidth: true

                        text: ((!root.editable)? qsTr("* Visit any cafe that support CheatCard Application"):
                                                 qsTr("Let's go create the first card! Press \"Add Card \" button"))


                        font.pointSize: 17
                        color: "#999999"
                        wrapMode: Label.WordWrap
                        horizontalAlignment: Text.AlignJustify
                        verticalAlignment: Text.AlignVCenter

                    }

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillWidth: true

                        text: qsTr("* At the time of purchase, tell the seller that you have a cheat card")

                        font.pointSize: 17
                        color: "#999999"
                        wrapMode: Label.WordWrap
                        horizontalAlignment: Text.AlignJustify
                        verticalAlignment: Text.AlignVCenter

                        visible: !root.editable
                    }

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillWidth: true

                        text: qsTr("* And get your bonuses!!!")

                        font.pointSize: 17
                        color: "#999999"
                        wrapMode: Label.WordWrap
                        horizontalAlignment: Text.AlignJustify
                        verticalAlignment: Text.AlignVCenter


                        visible: !root.editable
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

            }

            property int itemHeight: (itemWidth * 0.75)
            property int itemWidth: (root.editable)?
                                        Math.min(list.width, bottomButton.y / 0.75) :
                                        Math.min(list.width, list.height / 0.75)


            delegate: Component {
                Loader {
                    id: cardItem

                    asynchronous: true
                    sourceComponent: Component {
                        id: ecitCardSource
                        Item {
                            height: list.itemHeight
                            width: list.itemWidth
                            x: list.width / 2 - width / 2
                            y: list.height / 2 - height / 2

                            anchors.centerIn: parent

                            EditCardView {
                                id: cardView
                                isCurrentItem: cardItem.ListView.isCurrentItem

                                model: card
                                showSeals: !(vScrollBar.active || hScrollBar.active) && isCurrentItem
                                editable: !Boolean(card && card.title.length)
                                onEditableChanged: {
                                    hasEdit = editable
                                }
                                onFinished: () => {
                                                hasEdit = editable = customization = false
                                                root.finished()

                                            }

                                Behavior on width {
                                    NumberAnimation {
                                        id: animation
                                        easing.type: Easing.InQuad

                                    }
                                }

                                Behavior on height {
                                    NumberAnimation {
                                        easing.type: Easing.InQuad

                                    }
                                }

                                Behavior on opacity {
                                    NumberAnimation {
                                        easing.type: Easing.InQuad
                                    }
                                }
                                opacity: (isCurrentItem)? 1: 0.5

                                height: (isCurrentItem)? cardItem.height: cardItem.height * 0.9
                                width: (isCurrentItem)? cardItem.width : cardItem.width * 0.9
                                anchors.centerIn: parent

                            }
                        }
                    }
                }
            }
        }

        Button {
            id: bottomButton
            text: (cardCount)?  qsTr("Activate card") : qsTr("Add card")
            Layout.alignment: Qt.AlignHCenter
            visible: root.editable
            enabled: !hasEdit
            onClicked: () => {
                           if (cardCount) {
                               root.model.activateCardByIndex(list.currentIndex)
                           } else {
                               createCard()
                           }
                       }
        }
    }
}
