import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property var model: null
    property bool editable: true

    signal finished();

    contentItem: ColumnLayout {
        anchors.fill: parent

        Rectangle {
            id: cardRectangle

            property string seelTmpImage: ""


            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: (root.model)? root.model.color : "#777777"
            radius: 10
            clip: true
            Image {
                id: cardBackground
                source: "image://cards/background/" + ((root.model)? root.model.id : "0")
                anchors.fill: parent
            }

            GridLayout {
                anchors.fill: parent
                rows: 2
                columns: 2

                Image {
                    id: cardLogoIamge
                    fillMode: Image.PreserveAspectFit
                    source: "image://cards/logo/" + ((root.model)? root.model.id : "0")
                    Layout.alignment: Qt.AlignHCenter

                    Layout.rowSpan: 1
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 2

                    opacity: height > 50 && width > 50

                }

                GridLayout {
                    rows: 3
                    columns: 2
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop
                    Layout.rightMargin: 10
                    flow: GridLayout.TopToBottom

                    TextField {
                        id: cardTitle
                        Layout.columnSpan: 2
                        horizontalAlignment:  Text.AlignHCenter
                        Layout.fillWidth: true
                        text: (root.model)? root.model.title : ""
                        onTextChanged: {
                            if (!root.model)
                                return
                            root.model.title = text
                        }

                        placeholderText: qsTr("Enter card title");
                        readOnly: !editable
                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.telegramm : ""
                        textField.placeholderText: qsTr("Enter telegramm chennal name");
                        textField.readOnly: !editable
                        textField.onTextChanged: {
                            if (!root.model)
                                return

                            root.model.telegramm = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally("https://t.me/" + textField.text)
                                   }

                        image: "qrc:/images/private/resources/telegramm.png"
                        visible: root.editable || textField.text.length

                    }


                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.instagramm : ""
                        textField.placeholderText: qsTr("Enter your instagramm page");
                        textField.readOnly: !editable
                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.instagramm = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally("https://www.instagram.com/" + textField.text)
                                   }

                        image: "qrc:/images/private/resources/instagramm.png"
                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.physicalAddress : ""
                        textField.placeholderText: qsTr("Enter your physical address");
                        textField.readOnly: !editable
                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.physicalAddress = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally("https://www.google.com/maps/search/" + textField.text)
                                   }

                        image: "qrc:/images/private/resources/mapLogo.png"
                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {

                        textField.text: (root.model)? root.model.webSite : ""

                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.webSite = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally(textField.text)
                                   }

                        textField.placeholderText: qsTr("Enter your web site domain");
                        textField.readOnly: !editable

                        image: "qrc:/images/private/resources/www.png"

                        visible: root.editable || textField.text.length

                    }
                }

                GridLayout {
                    id: freeFridLayout

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

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

                                Layout.fillHeight: true
                                Layout.maximumHeight: cardTitle.height
                                Layout.preferredWidth: height
                                radius: Math.min(width, height) / 2

                                Label {
                                    visible: Boolean(index === (freeIndex.value - 1))
                                    text: qsTr("Free")
                                    font.bold: true
                                    anchors.centerIn: parent
                                }

                                Image {
                                    id: seelImage
                                    visible: root.model &&
                                             ((root.model.purchasesNumber %
                                               freeIndex.value) > index)

                                    anchors.centerIn: parent
                                    width: parent.width * 0.9
                                    height: parent.width * 0.9

                                    source: "image://cards/seal/" +
                                            ((root.model)? root.model.id : "0")

                                    Connections {
                                        target: cardRectangle

                                        function onSeelTmpImageChanged() {
                                            seelImage.source = cardRectangle.seelTmpImage
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }

        RowLayout {
            visible: editable
            Layout.alignment: Qt.AlignHCenter


            ToolButton {
                text: qsTr("⋮")
                font.bold: true
                font.pointSize: 14
                onClicked: () => {
                               customisationMenu.popup()
                           }
            }

            SpinBox {
                id: freeIndex
                value: (root.model)? root.model.freeIndex : privateRoot.rowSignCount
                stepSize: Math.ceil((freeIndex.value + 1) / privateRoot.rowSignCount)
                to: privateRoot.rowSignCount * privateRoot.maximumRowSignCount
                from: 2

                onValueChanged: () => {
                                    if (!root.model)
                                    return
                                    root.model.freeIndex = freeIndex.value
                                }
            }

            Button {
                text: qsTr("Save");
                enabled: cardTitle.text.length
                onClicked: () => {
                               if (root.model) {
                                   root.finished()
                                   root.model.save()
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


    Dialog {
        id: defaultImages

        ItemsView {
            title: qsTr("Select Image")
            id: sourceImages
            model: (mainModel)? mainModel.defaultBackgroundsModel: null

            anchors.fill: parent

        }

        width: root.width * 0.9
        height: root.height * 0.9

        anchors.centerIn: parent

        onAccepted: () => {
                        if (!root.model) {
                            return
                        };

                        cardBackground.source = sourceImages.currentSelectedItem
                        root.model.setNewBackGround(sourceImages.currentSelectedItem);
                    }

        standardButtons: Dialog.Open | Dialog.Close
    }

    Dialog {
        id: defaultLogos

        contentItem: ItemsView {
            id: sourceLogos
            model: (mainModel)? mainModel.defaultLogosModel: null

        }

        onAccepted: () => {
                        if (!root.model) {
                            return
                        };

                        cardLogoIamge.source = sourceLogos.currentSelectedItem
                        root.model.setNewLogo(sourceLogos.currentSelectedItem);
                    }

        anchors.centerIn: parent
        width: root.width * 0.9
        height: root.height * 0.9

        standardButtons: Dialog.Open | Dialog.Close
    }

    Dialog {
        id: defaultSeels

        contentItem: ItemsView {
            id: sourceSeels
            model: (mainModel)? mainModel.defaultLogosModel: null

        }

        onAccepted: () => {
                        if (!root.model) {
                            return
                        };

                        cardRectangle.seelTmpImage = sourceSeels.currentSelectedItem
                        root.model.setNewSeel(sourceSeels.currentSelectedItem);
                    }

        width: root.width * 0.9
        height: root.height * 0.9

        anchors.centerIn: parent

        standardButtons: Dialog.Open | Dialog.Close
    }


    Menu {
        id: customisationMenu

        MenuItem {
            text: qsTr("Change background color")

            onClicked: () => {
                           colorDialog.open()
                       }
        }

        MenuItem {
            text: qsTr("Change background image")

            onClicked: () => {
                           defaultImages.open()
                       }
        }

        MenuItem {
            text: qsTr("Change card logo")

            onClicked: () => {
                           defaultLogos.open()
                       }
        }

        MenuItem {
            text: qsTr("Change card seel")

            onClicked: () => {
                           defaultSeels.open()
                       }
        }
    }
}
