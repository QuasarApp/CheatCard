import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property var model: null
    property bool editable: true

    signal finished();

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: (root.model)? qsTr("Editing: ") + root.model.name: qsTr("No card selected")
        font.bold: true
        visible: editable
    }

    contentItem: ColumnLayout {
        anchors.fill: parent

        Rectangle {
            id: cardRectangle
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: (root.model)? root.model.color : "#777777"
            radius: 10

            GridLayout {
                anchors.fill: parent
                rows: 2
                columns: 2

                Image {

                    fillMode: Image.PreserveAspectFit
                    source: "image://cards/logo/" + ((root.model)? root.model.id : "0")
                    Layout.alignment: Qt.AlignHCenter

                    Layout.rowSpan: 1
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 2


                }

                GridLayout {
                    rows: 4
                    columns: 2
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop
                    Layout.rightMargin: 10

                    TextField {
                        id: cardTitle
                        Layout.columnSpan: 2
                        horizontalAlignment:  Text.AlignHCenter
                        Layout.fillWidth: true
                        text: (root.model)? root.model.title : ""
                        placeholderText: qsTr("Enter card title");
                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.telegramm : ""
                        textField.placeholderText: qsTr("Enter telegramm chennal name");
                        image: "qrc:/images/private/resources/telegramm.png"
                    }


                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.instagramm : ""
                        textField.placeholderText: qsTr("Enter your instagramm page");
                        image: "qrc:/images/private/resources/instagramm.png"

                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.physicalAddress : ""
                        textField.placeholderText: qsTr("Enter your physical address");
                        image: "qrc:/images/private/resources/mapLogo.png"

                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.webSite : ""
                        textField.placeholderText: qsTr("Enter your web site domain");
                        image: "qrc:/images/private/resources/www.png"

                    }
                }

                Item {
                    Layout.rowSpan: 1
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GridLayout {
                        id: freeFridLayout
                        anchors.fill: parent

                        columns: freeIndex.value / rows
                        rows: Math.ceil(freeIndex.value / privateRoot.rowSignCount)

                        Repeater {
                            id: reppit
                            model:  freeIndex.value
                            delegate: signZone
                            Component {
                                id: signZone
                                Rectangle {
                                    Layout.alignment: Qt.AlignHCenter

                                    width: (cardTitle.height * 1.5) / freeFridLayout.rows
                                    height: width
                                    radius: Math.min(width, height) / 2

                                    Label {
                                        visible: Boolean(index === (freeIndex.value - 1))
                                        text: qsTr("Free")
                                        font.bold: true
                                        anchors.centerIn: parent
                                    }

                                    Image {
                                        visible: root.model &&
                                                 ((root.model.purchasesNumber %
                                                  freeIndex.value) > index)

                                        anchors.centerIn: parent
                                        width: parent.width * 0.9
                                        height: parent.width * 0.9

                                        source: "image://cards/seal/" +
                                                ((root.model)? root.model.id : "0")
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Button {
                text: qsTr("Pick card color");

                onClicked: () => {
                               colorDialog.open()
                           }
            }

            Label {
                text: qsTr("Free index: ")
            }

            SpinBox {
                id: freeIndex
                value: (root.model)? root.model.freeIndex : privateRoot.rowSignCount
                stepSize: Math.ceil((freeIndex.value + 1) / privateRoot.rowSignCount)
                to: privateRoot.rowSignCount * privateRoot.maximumRowSignCount
                from: 2

            }


            Button {
                text: qsTr("Save");
                enabled: cardTitle.text.length
                onClicked: () => {
                               if (root.model) {
                                   root.model.finished()
                               }
                           }
            }
        }


        Item {
            id: privateRoot

            property int rowSignCount: 6
            property int maximumRowSignCount: 3

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
