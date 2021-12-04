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
                            activityCard();
                        }

                        onSigSwipe: (side) => {
                                        statisticsCard();
                                        swipeCard(side);
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

                    ToolButton {
                        id: editCardBtn
                        visible: cardItem.ListView.isCurrentItem && !cardView.editable
                        icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                        font.bold: true
                        font.pointSize: 14

                        onClicked: editMenu.popup(this, editCardBtn.x, editCardBtn.height)
                    }

                    Menu {
                        id: editMenu

                        MenuItem {

                            visible: (mainModel)? mainModel.mode: false
                            height: (visible)? implicitHeight : 0

                            text: qsTr("Edit card")
                            icon.source: "qrc:/images/private/resources/Interface_icons/edit_card.svg"
                            onClicked:  () => {
                                            cardView.editable = true;
                                        }
                        }

                        MenuItem {

                            text: qsTr("Remove card")
                            icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                            onClicked:  () => {
                                            root.model.deleteCard(card.id);
                                        }
                        }

                        MenuItem {

                            visible: (mainModel)? mainModel.mode: false
                            height: (visible)? implicitHeight : 0

                            text: qsTr("Activate card")
                            icon.source: "qrc:/images/private/resources/Interface_icons/Activate.svg"
                            onClicked:  () => {
                                            activityCard();
                                        }
                        }

                        MenuItem {

                            text: qsTr("Statistics")
                            icon.source: "qrc:/images/private/resources/Interface_icons/statistic.svg"
                            onClicked: (side) => {
                                           if (!root.model.mode) {
                                               turnOverCard(side);
                                           } else {
                                               showStatisticsCard();
                                           }

                                        }
                        }        

                    }

                    function activityCard() {
                        if (root.model) {
                            root.model.cardSelected(card.id, 0)
                        }

                        const fAvailable = root.editable && !cardView.editable && cardItem.ListView.isCurrentItem;
                        if (!fAvailable) {
                            return;
                        }

                        activityProcessor.newActivity("qrc:/CheatCardModule/WaitConnectView.qml",
                                                      mainModel.waitModel)
                    }

                    function showStatisticsCard() {
                        if (root.editable) {

                            if (root.model) {
                                root.model.cardSelected(card.id, 1)
                            }

                            const activity = "qrc:/CheatCardModule/SellerStatistic.qml";
                            activityProcessor.newActivity(activity,
                                                          mainModel.statisticModel)
                            return;
                        }
                    }

                    function turnOverCard(s) {

                        if (list.orientation === ListView.Vertical ||
                            s === 2 || s === 3) {

                            cardView.turnOverCard(list.orientation === ListView.Vertical);
                        } else {
                            cardView.turnOverCard(list.orientation === ListView.Vertical);
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
