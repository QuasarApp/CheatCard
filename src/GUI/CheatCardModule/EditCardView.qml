import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "Style"

Page {
    id: root
    property var model: null
    property string fontColor: (model)? model.fontColor : "#000000"
    property bool editable: true

    property int  purchasesNumber: (model)? model.purchasesNumber: 1
    property int freeIndexCount :(model)? model.freeIndex: 0
    property int receivedItems :(mainModel && model)? mainModel.getReceivedItemsCount(model.id): 0

    property bool backSide: false

    signal finished();
    signal sigHold();

    // 0 - righ to left:
    // 1 - left to right
    // 2 - top to bottom
    // 3 - bottom to top
    signal sigSwipe(var side);

    function turnOverCard(vertical) {


        rotationObjectBackSide.yAxis = vertical
        rotationObjectBackSide.xAxis = !vertical

        rotationObject.yAxis = vertical
        rotationObject.xAxis = !vertical

        rotationObject.angle = 180 * !root.backSide
        root.backSide = !root.backSide;
    }

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
                fillMode: Image.PreserveAspectCrop
                source: "image://cards/background/" + ((root.model)? root.model.id : "0")
                anchors.fill: parent
            }

            MouseArea {
                property real previousPositionX: 0
                property real previousPositionY: 0
                property real directionX: 0
                property real directionY: 0

                anchors.fill: parent;

                onPressed: (mouse) => {
                               previousPositionX = mouseX;
                               previousPositionY = mouseY;

                               directionX = 0;
                               directionY = 0;

                           }

                onPositionChanged: (mouse) => {
                                       directionX = mouseX - previousPositionX;
                                       directionY = mouseY - previousPositionY
                                   }

                onReleased: (mouse) => {

                                if(directionX > directionY) {
                                    root.sigSwipe(directionX > 0)
                                } else {
                                    root.sigSwipe(2 + directionY > 0)
                                }

                            }

                onPressAndHold: (mouse) => {
                                    root.sigHold();
                                }
            }


            GridLayout {
                id: frontSide;

                anchors.fill: parent
                rows: 2
                columns: 2
                visible: !root.backSide
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

                    CTextField {
                        id: cardTitle
                        color: root.fontColor
                        fontColor: root.fontColor
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

                        textField.color: fontColor
                        lineColor: fontColor

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

                        textField.color: fontColor
                        lineColor: fontColor

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

                        textField.color: fontColor
                        lineColor: fontColor

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

                        textField.color: fontColor
                        lineColor: fontColor
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

                        textField.color: fontColor
                        lineColor: fontColor
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

                        textField.color: fontColor
                        lineColor: fontColor
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

            GridLayout {
                id: backSide;
                visible: root.backSide
                anchors.fill: parent
                rows: 4
                columns: 4

                transform: Rotation {
                    id: rotationObjectBackSide

                    property int xAxis: 0
                    property int yAxis: 0

                    origin.x: backSide.width / 2;
                    origin.y: backSide.height / 2;
                    axis {
                        x: xAxis; y: yAxis; z: 0 }
                    angle: 180

                }

                Item {
                    Layout.fillWidth: true
                    Layout.rowSpan: 4
                }

                CTextField {
                    color: root.fontColor
                    fontColor: root.fontColor
                    Layout.columnSpan: 2

                    horizontalAlignment:  Text.AlignHCenter
                    Layout.fillWidth: true
                    text: (root.model)? qsTr("Detail of ") + root.model.title : ""

                    readOnly: true
                }

                Item {
                    Layout.fillWidth: true
                    Layout.rowSpan: 4
                }

                Label {
                    color: root.fontColor

                    text: qsTr("Purchases count: ")
                }

                CTextField {
                    text: purchasesNumber
                    horizontalAlignment:  Text.AlignHCenter
                    fontColor: root.fontColor

                }

                Label {
                    color: root.fontColor

                    text: qsTr("Available %0: ").arg((model)? model.freeItem: "");
                }

                CTextField {
                    text: Math.floor(purchasesNumber / freeIndexCount) - receivedItems
                    horizontalAlignment:  Text.AlignHCenter
                    fontColor: root.fontColor

                }

                Label {
                    color: root.fontColor

                    text: qsTr("Received %0: ").arg((model)? model.freeItem: "");
                }

                CTextField {
                    text:  receivedItems
                    horizontalAlignment:  Text.AlignHCenter
                    fontColor: root.fontColor

                }

                Item {
                    Layout.fillHeight: true
                }
            }


            transform: Rotation {
                id: rotationObject

                property int xAxis: 0
                property int yAxis: 0

                origin.x: cardRectangle.width / 2;
                origin.y: cardRectangle.height / 2;
                axis {
                    x: xAxis; y: yAxis; z: 0 }
                angle: 0

                Behavior on angle {
                    NumberAnimation {

                    }
                }
            }
        }
        RowLayout {
            visible: editable
            Layout.alignment: Qt.AlignHCenter


            ToolButton {
                id: menuButton
                text: qsTr("⋮")
                font.bold: true
                font.pointSize: 14
                onClicked: () => {
                               customisationMenu.popup(this, menuButton.x, menuButton.height)
                           }
            }

            SpinBox {
                id: freeIndex
                value: (root.model)? freeIndexCount : privateRoot.rowSignCount
                stepSize: Math.ceil((freeIndex.value + 1) / privateRoot.rowSignCount)
                to: privateRoot.rowSignCount * privateRoot.maximumRowSignCount
                from: 2

                onValueChanged: () => {
                                    if (!root.model)
                                    return freeIndexCount = freeIndex.value
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
            implicitHeight: 0x0
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

    Component {
        id: defaultColorFont

        ColorPicker {
            id: colorPickFont

            header: Label {
                horizontalAlignment: Label.AlignHCenter
                text: qsTr("Please choose a color")
                font.bold: true
            }

            footer: DialogButtonBox {
                onAccepted: () => {
                                if (root.model) {
                                    fontColor = colorPickFont.color
                                    root.model.fontColor = colorPickFont.color
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
            text: qsTr("Changed foreground color")
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultColorFont);

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
