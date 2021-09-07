import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property var model: null

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
            preferredHighlightBegin: list.height / 2 - itemHeight / 2
            preferredHighlightEnd: preferredHighlightBegin
            highlightRangeMode: ListView.StrictlyEnforceRange
            ScrollBar.vertical: ScrollBar {}


            property int itemHeight: (itemWidth * 0.5)
            property int itemWidth: Math.min(list.width, list.height)

            Component {
                id: delegateItem

                Item {
                    id: cardItem
                    height: (cardItem.ListView.isCurrentItem)? list.itemHeight * 1.3: list.itemHeight
                    width: list.itemWidth
                    x: list.width / 2 - width / 2

                    EditCardView {
                        id: cardView
                        model: card
                        clip: true
                        opacity: (cardItem.ListView.isCurrentItem)? 1: 0.5
                        editable: false
                        onFinished: () => {
                                        editable = false
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
                        text: qsTr("✎")
                        font.bold: true
                        visible:  cardItem.ListView.isCurrentItem && !cardView.editable
                        onClicked: () => {
                                        cardView.editable = true
                                   }
                    }
                }


            }

            delegate: delegateItem
        }

        Button {
            text: qsTr("Add card")
            Layout.alignment: Qt.AlignHCenter

            onClicked: () => {
                           inputName.open()
                       }
        }

    }

    Dialog {
        id: inputName;

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        TextField {
            id: input
            placeholderText: qsTr("Enter new nama");
            Layout.alignment: Qt.AlignHCenter

        }

        Component.onCompleted: {
            var applayButton = standardButton(Dialog.Ok)

            applayButton.enabled = input.length
            input.onTextChanged.connect(()=> {applayButton.enabled = input.length})
        }

        onAccepted: () => {
                        if (!root.model) {
                            return;
                        }

                        root.model.addCard(input.text)
                    }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }
}
