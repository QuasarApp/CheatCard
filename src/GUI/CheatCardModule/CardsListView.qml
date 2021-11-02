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

Page {
    id: root
    property var model: null
    property bool editable: true
    property alias cardCount: list.count

    signal finished()

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: list
            model: root.model
            currentIndex: count - 1;

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
            ScrollBar.vertical: ScrollBar {}

            orientation: (root.width < root.height)? ListView.Vertical : ListView.Horizontal

            Label {
                text: qsTr("Don't have any cards yet? ") +
                      ((!root.editable)? qsTr("Visit any coffee and start receiving bonuses now!"):
                                         qsTr("Let's go create the first card! Press \"Add Card \" button"))

                font.pointSize: 20
                color: "#999999"
                wrapMode: Label.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                visible: !list.count
                anchors.fill: parent
            }


            property int itemHeight: (itemWidth * 0.75)
            property int itemWidth: (root.editable)?
                                        Math.min(list.width, Math.min(buttonAddCard.y, ) / 0.75) : Math.min(list.width, list.height / 0.75)

            Component {
                id: delegateItem

                Item {
                    id: cardItem
                    height: list.itemHeight
                    width: list.itemWidth
                    x: list.width / 2 - width / 2
                    y: list.height / 2 - height / 2

                    EditCardView {
                        id: cardView
                        model: card
                        opacity: (cardItem.ListView.isCurrentItem)? 1: 0.5
                        editable: false
                        onFinished: () => {
                                        editable = false
                                        root.finished()
                                    }

                        onSigHold: {
                            if (root.model) {
                                root.model.cardSelected(card.id)
                            }

                            const fAvailable = root.editable && !cardView.editable && cardItem.ListView.isCurrentItem;
                            if (!fAvailable) {
                                return;
                            }

                            activityProcessor.newActivity("qrc:/CheatCardModule/WaitConnectView.qml",
                                                          mainModel.waitModel)
                        }

                        onSigSwipe: (side) => {

                                        if (root.editable)
                                            return;

                                        if (list.orientation === ListView.Vertical ||
                                            side === 2 || side === 3) {

                                            turnOverCard(list.orientation === ListView.Vertical);
                                        } else {
                                            turnOverCard(list.orientation === ListView.Vertical);
                                        }
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

                        height: parent.height * ((cardItem.ListView.isCurrentItem)? 1: 0.9)
                        width: parent.width * ((cardItem.ListView.isCurrentItem)? 1: 0.9)
                        anchors.centerIn: parent
                    }

                    Button {
                        text: qsTr("Edit")

                        font.bold: true
                        visible:  cardItem.ListView.isCurrentItem && !cardView.editable && root.editable
                        onClicked: () => {
                                       cardView.editable = true
                                   }
                    }
                }
            }

            delegate: delegateItem
        }

        Button {
            id: buttonAddCard
            text: qsTr("Add card")
            Layout.alignment: Qt.AlignHCenter
            visible: root.editable
            onClicked: () => {
                           root.model.addCard()

                       }
        }
    }
}
