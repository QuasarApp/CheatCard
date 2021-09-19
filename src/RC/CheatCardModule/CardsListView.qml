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
            currentIndex: 0;
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            snapMode: ListView.NoSnap
            boundsBehavior:Flickable.StopAtBounds
            preferredHighlightBegin: (orientation == ListView.Vertical)?
                                         list.height / 2 - itemHeight / 2 :
                                         list.width / 2 - itemWidth / 2
            preferredHighlightEnd: preferredHighlightBegin

            highlightRangeMode: ListView.StrictlyEnforceRange
            ScrollBar.vertical: ScrollBar {}

            orientation: (root.width < root.height)? ListView.Vertical : ListView.Horizontal

            Label {
                text: qsTr("You do not have any card. ") +
                      (root.editable)? qsTr("Visit any coffee to get a new card and their bonuses"):
                                       qsTr("Please create a new card for work. Press \"Add Card \" button")

                font.pointSize: 20
                color: "#999999"
                wrapMode: Label.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                visible: !list.count
                anchors.fill: parent
            }

            property int itemHeight: (itemWidth * 0.75)
            property int itemWidth: Math.min(list.width, buttonAddCard.y / 0.75)

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
                        clip: true
                        opacity: (cardItem.ListView.isCurrentItem)? 1: 0.5
                        editable: false
                        onFinished: () => {
                                        editable = false
                                        root.finished()
                                    }

                        TapHandler {
                            enabled: !cardView.editable
                            onLongPressed:  {

                                if (root.model) {
                                    root.model.cardSelected(card.id)
                                }
                                waitConnect.open()
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

    Dialog {
        id: waitConnect

        contentItem: WaitConnectView {
            id: waitConnectView
            model: (mainModel)? mainModel.waitModel: null

            onComplete: {
                waitConnect.close();
            }
        }

        anchors.centerIn: parent
        width: root.width * 0.9
        height: root.height * 0.9

    }
}
