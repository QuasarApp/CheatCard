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
                id: scrollBar
                enabled: !hasEdit
            }

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
                        opacity: (isCurrentItem)? 1: 0.5
                        editable: !Boolean(card && card.title.length)
                        isCurrentItem: cardItem.ListView.isCurrentItem
                        onFinished: () => {
                                        hasEdit = editable = false
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

                        height: parent.height * ((cardItem.ListView.isCurrentItem)? 1: 0.9)
                        width: parent.width * ((cardItem.ListView.isCurrentItem)? 1: 0.9)
                        anchors.centerIn: parent
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
            enabled: !hasEdit
            onClicked: () => {
                           root.model.addCard()
                           hasEdit = true;

                       }
        }
    }

}
