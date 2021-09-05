import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property var model: null

    signal finished();

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: (model)? qsTr("Editing: ") + model.name: qsTr("No card selected")
        font.bold: true
    }

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
            }

            Rectangle {
                id: cardRectangle
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.preferredHeight: width * 0.4

                color: (model)? model.color : "#777777"
                radius: 10

                GridLayout {
                    anchors.fill: parent

                    Image {
                        source: "image://cards/logo/" + ((root.model)? root.model.id : "0")
                    }

                    Image {
                        source: "image://cards/seal/" + ((root.model)? root.model.id : "0")
                    }

                    TextField {
                        id: cardName
                        text: (root.model)? root.model.name : ""
                        placeholderText: qsTr("Enter card name");
                    }

                    TextField {
                        text: (root.model)? root.model.telegramm : ""
                        placeholderText: qsTr("Enter telegramm chennal name");
                    }

                    TextField {
                        text: (root.model)? root.model.instagramm : ""
                        placeholderText: qsTr("Enter your instagramm page");
                    }

                    TextField {
                        text: (root.model)? root.model.physicalAddress : ""
                        placeholderText: qsTr("Enter your physical address");
                    }

                    TextField {
                        text: (root.model)? root.model.webSite : ""
                        placeholderText: qsTr("Enter your web site domain");
                    }
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Pick card color");

                onClicked: () => {
                               colorDialog.open()
                           }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Save");
                enabled: cardName.text.length
                onClicked: () => {
                               if (root.model) {
                                   root.model.finished()
                               }
                           }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    ColorPicker {
        id: colorDialog

        header: Label {
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("Please choose a color")
            font.bold: true
        }

        onAccepted: {
            if (root.model) {
                cardRectangle.color = colorDialog.color
                root.model.color = colorDialog.color
            }

            close()
        }
        onRejected: {
            close()
        }
    }
}
