import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property var model: null
    property bool editable: true
    property int  purchasesNumber: (model)? model.purchasesNumber: 1
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
                    rows: 3 + (visibleItemsCount <= 3 ) + (editable || visibleItemsCount > 4 )
                    columns: (visibleItemsCount <= 3 )? 1: 2
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop
                    Layout.rightMargin: 10
                    flow: GridLayout.TopToBottom

                    property int visibleItemsCount:
                        cardTelegramm.visible +
                        cardInstagramm.visible +
                        cardphysicalAddress.visible +
                        cardwebSite.visible +
                        cardphone.visible +
                        cardfreeItem.visible

                    TextField {
                        id: cardTitle
                        Layout.columnSpan: parent.columns
                        horizontalAlignment:  Text.AlignHCenter
                        Layout.fillWidth: true
                        text: (root.model)? root.model.title : ""
                        onTextChanged: {
                            if (!root.model)
                                return
                            root.model.title = text
                        }

                        placeholderText: qsTr("Enter card title");
                        placeholderTextColor: "#c12300"
                        readOnly: !editable
                    }

                    TextFieldWithLogo {
                        id: cardTelegramm

                        textField.text: (root.model)? root.model.telegramm : ""
                        textField.placeholderText: qsTr("Your telegramm");
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
                        id: cardInstagramm

                        textField.text: (root.model)? root.model.instagramm : ""
                        textField.placeholderText: qsTr("Your instagramm");
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
                        id: cardphysicalAddress

                        textField.text: (root.model)? root.model.physicalAddress : ""
                        textField.placeholderText: qsTr("Your physical address");
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
                        id: cardwebSite
                        textField.text: (root.model)? root.model.webSite : ""

                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.webSite = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally(textField.text)
                                   }

                        textField.placeholderText: qsTr("Your web site");
                        textField.readOnly: !editable

                        image: "qrc:/images/private/resources/www.png"

                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {
                        id: cardphone

                        textField.text: (root.model)? root.model.phone : ""

                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.phone = textField.text
                        }

                        onClicked: () => {
                                       Qt.openUrlExternally("tel:" + textField.text)
                                   }

                        textField.placeholderText: qsTr("Your phone number");
                        textField.readOnly: !editable

                        image: "qrc:/images/private/resources/phone.png"

                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {
                        id: cardfreeItem

                        textField.text: (root.model)? root.model.freeItem : ""

                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.freeItem = textField.text
                        }

                        textField.placeholderText: qsTr("Enter bonus name");
                        textField.placeholderTextColor: "#c12300"

                        textField.readOnly: !editable

                        image: "qrc:/images/private/resources/freeItem.png"

                        visible: root.editable

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
                                    text: "Free"
                                    font.bold: true
                                    anchors.centerIn: parent
                                }

                                Image {
                                    id: seelImage
                                    mipmap: true;
                                    visible: Boolean(root.model) &&
                                             ((root.purchasesNumber %
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
                enabled: cardTitle.text.length && cardfreeItem.textField.text.length
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


    Component {
        id: selectImage
        ItemsView {
            title: qsTr("Select Image")
            id: sourceImages
            model: (mainModel)? mainModel.defaultBackgroundsModel: null

            footer: DialogButtonBox {
                onAccepted: () => {
                                if (!root.model) {
                                    return
                                };

                                cardBackground.source = sourceImages.currentSelectedItem
                                root.model.setNewBackGround(sourceImages.currentSelectedItem);
                                activityProcessor.popItem();
                            }

                standardButtons: Dialog.Open
            }
        }
    }

    Component {
        id: defaultLogos
        ItemsView {
            title: qsTr("Select card logo")
            id: sourceLogos
            model: (mainModel)? mainModel.defaultLogosModel: null

            footer: DialogButtonBox {
                onAccepted: () => {
                                if (!root.model) {
                                    return
                                };

                                cardLogoIamge.source = sourceLogos.currentSelectedItem
                                root.model.setNewLogo(sourceLogos.currentSelectedItem);
                                activityProcessor.popItem();

                            }

                standardButtons: Dialog.Open
            }
        }
    }

    Component {
        id: defaultSeels
        ItemsView {
            title: qsTr("Select card seal")
            id: sourceSeels
            model: (mainModel)? mainModel.defaultLogosModel: null

            footer: DialogButtonBox {
                onAccepted: () => {
                                if (!root.model) {
                                    return
                                };

                                cardRectangle.seelTmpImage = sourceSeels.currentSelectedItem
                                root.model.setNewSeel(sourceSeels.currentSelectedItem);
                                activityProcessor.popItem();

                            }

                standardButtons: Dialog.Open
            }
        }
    }

    Component {
        id: defaultColor


        ColorPicker {
            id: colorPick

            header: Label {
                horizontalAlignment: Label.AlignHCenter
                text: qsTr("Please choose a color")
                font.bold: true
            }

            footer: DialogButtonBox {
                onAccepted: () => {
                                if (root.model) {
                                    cardRectangle.color = colorPick.color
                                    root.model.color = colorPick.color
                                }
                                activityProcessor.popItem();

                            }

                standardButtons: Dialog.Open
            }
        }
    }

    Menu {
        id: customisationMenu

        MenuItem {
            text: qsTr("Change background color")
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultColor);

                       }
        }

        MenuItem {
            text: qsTr("Change background image")

            onClicked: () => {
                           activityProcessor.newActivityFromComponent(selectImage);

                       }
        }

        MenuItem {
            text: qsTr("Change card logo")

            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultLogos);

                       }
        }

        MenuItem {
            text: qsTr("Change card seal")

            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultSeels);
                       }
        }
    }
}
