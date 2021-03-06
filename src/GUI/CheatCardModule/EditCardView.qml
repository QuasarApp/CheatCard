//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "Style"

Page {
    id: root
    property var model: null
    property string fontColor: ""
    Binding {
        target: root
        property: "fontColor"
        value: (root.model)? model.fontColor : "#333333"
    }
    property bool editable: true
    property bool creating: false

    property bool customization: false
    property bool cardInteractive: true

    property int purchasesNumber: (model)? model.purchasesNumber: 1
    property int freeIndexCount :(model)? model.freeIndex: 0
    property int receivedItems: (model)? model.receivedItems: 0
    property int available: (model)? model.available: 0

    property bool backSide: false
    property bool isCurrentItem: false

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

            Binding {
                target: cardRectangle
                property: "color"
                value: (root.model)? root.model.color : "#777777"
            }
            radius: 10
            clip: false

            Image {
                id: cardBackground
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent

                Binding {
                    target: cardBackground
                    property: "source"
                    value: "image://cards/background:" + ((root.model)? root.model.id  + ":" + root.model.cardVersion: "0")
                }

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: cardBackground.width
                        height: cardBackground.height
                        radius: cardRectangle.radius
                    }
                }
            }

            MouseArea {
                property real previousPositionX: 0
                property real previousPositionY: 0
                property real directionX: 0
                property real directionY: 0

                function swipe(side) {
                    root.sigSwipe(side);

                    if (!mainModel.mode) {
                        privateRoot.turnOverCard(side);
                    } else {
                        privateRoot.showStatisticsCard();
                    }
                }

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

                                const width = 20;

                                if(directionX > directionY) {
                                    if (directionX > width)
                                        swipe(directionX > 0)
                                } else {
                                    if (directionY > width)
                                        swipe(2 + (directionY > 0))
                                }

                            }

                onPressAndHold: (mouse) => {
                                    root.sigHold();
                                    privateRoot.activityCard();
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
                    Binding {
                        target: cardLogoIamge
                        property: "source"
                        value: "image://cards/logo:" + ((root.model)? root.model.id  + ":" + root.model.cardVersion : "0")

                    }
                    Layout.alignment: Qt.AlignHCenter

                    Layout.rowSpan: 1
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 2

                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                            width: cardLogoIamge.width
                            height: cardLogoIamge.height
                            radius: 25
                        }
                    }

                    opacity: height > 50 && width > 50

                }

                GridLayout {
                    rows: 3 + (visibleItemsCount <= 3 ) + (root.editable || visibleItemsCount > 4 )
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
                        horizontalAlignment:  Label.AlignHCenter
                        Layout.fillWidth: true
                        text: (root.model)? root.model.title : ""
                        onTextChanged: {
                            if (!root.model)
                                return
                            root.model.title = text
                        }

                        placeholderText: qsTr("Enter card title");
                        placeholderTextColor: "#c12300"
                        readOnly: !root.editable
                    }

                    TextFieldWithLogo {
                        id: cardTelegramm

                        textField.color: fontColor
                        lineColor: fontColor

                        textField.text: (root.model)? root.model.telegramm : ""
                        textField.placeholderText: qsTr("Your telegramm");
                        textField.readOnly: !root.editable
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
                        textField.readOnly: !root.editable
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
                        textField.readOnly: !root.editable
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
                        textField.readOnly: !root.editable

                        image: "qrc:/images/private/resources/www.png"

                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {
                        id: cardphone

                        textField.color: root.fontColor
                        lineColor: root.fontColor
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
                        textField.readOnly: !root.editable

                        image: "qrc:/images/private/resources/phone.png"

                        visible: root.editable || textField.text.length

                    }

                    TextFieldWithLogo {
                        id: cardfreeItem

                        textField.color: root.fontColor
                        lineColor: root.fontColor
                        textField.text: (root.model)? root.model.freeItem : ""

                        textField.onTextChanged: {
                            if (!root.model)
                                return
                            root.model.freeItem = textField.text
                        }

                        textField.placeholderText: qsTr("Enter bonus name");
                        textField.placeholderTextColor: "#c12300"

                        textField.readOnly: !root.editable

                        image: "qrc:/images/private/resources/freeItem.png"

                        visible: root.editable

                    }
                }

                GridLayout {
                    id: freeFridLayout

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    columns: signCount / rows
                    rows: Math.ceil(freeIndex.value / privateRoot.rowSignCount)

                    property int signCount: freeIndex.value + Number(freeRound.visible)

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

                                Image {
                                    id: seelImage
                                    mipmap: true;
                                    visible: Boolean(root.model) &&
                                             ((root.purchasesNumber %
                                               freeIndex.value) > index)

                                    anchors.centerIn: parent
                                    width: parent.width * 0.9
                                    height: parent.width * 0.9
                                    fillMode: Image.PreserveAspectFit

                                    Binding {
                                        target: seelImage
                                        property: "source"
                                        value: "image://cards/seal:" +
                                               ((root.model)? root.model.id  + ":" + root.model.cardVersion: "0")

                                    }

                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: Rectangle {
                                            width: seelImage.width
                                            height: seelImage.height
                                            radius: Math.min(seelImage.height, seelImage.width)
                                        }
                                    }

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

                    Rectangle {
                        id: freeRound
                        Layout.alignment: Qt.AlignHCenter

                        Layout.fillHeight: true
                        Layout.maximumHeight: cardTitle.height
                        Layout.preferredWidth: height
                        radius: Math.min(width, height) / 2

                        visible: freeIndex.value > privateRoot.rowSignCount && freeIndex.value % 2

                        Label {
                            text: "Free"
                            font.bold: true
                            color: "#000000"
                            anchors.centerIn: parent
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

                    horizontalAlignment:  Label.AlignHCenter
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
                    horizontalAlignment:  Label.AlignHCenter
                    fontColor: root.fontColor
                    readOnly: true

                }

                CLabel {
                    Layout.fillWidth: true
                    color: root.fontColor
                    text: qsTr("Available %0: ").arg((model)? model.freeItem: "");
                }

                CTextField {
                    text: available
                    horizontalAlignment:  Label.AlignHCenter
                    fontColor: root.fontColor
                    readOnly: true

                }

                CLabel {
                    color: root.fontColor
                    Layout.fillWidth: true

                    text: qsTr("Received %0: ").arg((model)? model.freeItem: "");
                }

                CTextField {
                    text:  receivedItems
                    horizontalAlignment:  Label.AlignHCenter
                    fontColor: root.fontColor
                    readOnly: true

                }

                Item {
                    Layout.fillHeight: true
                }
            }

            ToolButton {
                id: editCardBtn
                visible: isCurrentItem && !(root.editable || root.customization)
                icon.source: "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                icon.color: (root.model)? root.model.fontColor: Material.foreground
                font.bold: true
                font.pointSize: 14

                onClicked: editMenu.popup(this, editCardBtn.x, editCardBtn.height)
            }

            ToolButton {
                id: cancelCardBtn
                visible: (root.editable || root.customization) && !root.creating
                icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
                icon.color: (root.model)? root.model.fontColor: Material.foreground
                font.bold: true
                font.pointSize: 14

                onClicked: () => {
                               root.finished()
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
                        easing.period: 0.50
                        easing.amplitude: 1
                        easing.type: Easing.OutElastic
                        duration: 1500
                    }
                }
            }
        }
        RowLayout {
            visible: customization || root.editable
            Layout.alignment: Qt.AlignHCenter
            Layout.rightMargin: 10

            ToolButton {
                id: menuButton
                visible: customization || root.editable
                icon.source:  "qrc:/images/private/resources/Interface_icons/Right_topmenu.svg"
                font.bold: true
                font.pointSize: 14
                onClicked: () => {
                               customisationMenu.popup(this, menuButton.x, menuButton.height)
                           }
            }

            Item {
                Layout.fillWidth: true
            }

            SpinBox {
                id: freeIndex
                visible: root.editable
                value: (root.model)? root.model.freeIndex : privateRoot.rowSignCount
                stepSize: 1
                to: privateRoot.rowSignCount * privateRoot.maximumRowSignCount
                from: 2

                onValueChanged: () => {
                                    if (!root.model)
                                        return

                                    root.model.freeIndex = freeIndex.value
                                }
            }

            Button {
                text: qsTr("Default");
                visible: !freeIndex.visible
                enabled: cardTitle.text.length && cardfreeItem.textField.text.length
                onClicked: () => {
                               if (root.model) {
                                   root.finished()
                                   root.model.cardReset()
                               }
                           }
            }

            Button {
                text: qsTr("Save");
                visible: customization || root.editable
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

            property int rowSignCount: 7
            property int maximumRowSignCount: 2

            function showStatistickAction() {
                if (mainModel && mainModel.mode) {
                    showStatisticsCard();
                } else {
                    turnOverCard();
                }
            }

            function activityCard() {

                const fAvailable = mainModel.mode && isCurrentItem && cardInteractive && root.model;
                if (!fAvailable) {
                    return;
                }

                root.model.activate()
            }

            function showStatisticsCard() {
                if (!cardInteractive) {
                    return;
                }

                if (!root.editable) {

                    if (root.model) {
                        root.model.showStatistick()
                    }
                    return;
                }
            }

            function turnOverCard(s) {
                if (!cardInteractive) {
                    return;
                }
                root.turnOverCard(true);
            }
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

                                const selectdeItem = sourceImages.currentSelectedItem;

                                cardBackground.source = selectdeItem
                                root.model.setNewBackGround(selectdeItem);
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

                                const selectdeItem = sourceLogos.currentSelectedItem;
                                cardLogoIamge.source = selectdeItem
                                root.model.setNewLogo(selectdeItem);
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

                                const selectdeItem = sourceSeels.currentSelectedItem;

                                cardRectangle.seelTmpImage = selectdeItem
                                root.model.setNewSeel(selectdeItem);
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
            currentlyColor: (root.model)? root.model.color: "#000000"
            implicitHeight: 0x0
            title: qsTr("Please choose a color")


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
            currentlyColor: (root.model)? root.model.fontColor: "#000000"

            title: qsTr("Please choose a color")

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


    CMenu {
        id: editMenu

        MenuItem {

            visible: (root.model)? !(root.model.isMaster && mainModel.mode) : true
            height: (visible)? ganeralMenuItem.height: 0

            text: qsTr("Edit card")
            icon.source: "qrc:/images/private/resources/Interface_icons/edit_card.svg"
            onClicked:  () => {
                            if(mainModel.mode)
                                hasEdit = cardView.editable = true;
                            else
                                hasEdit = cardView.customization = true;
                        }
        }

        MenuItem {

            text: qsTr("Remove card")

            icon.source: "qrc:/images/private/resources/Interface_icons/delete_card.svg"
            onClicked:  () => {
                            if (root.model) {
                                root.model.remove();
                            }
                        }
        }

        MenuItem {

            visible: (mainModel)? mainModel.mode: false
            height: (visible)? ganeralMenuItem.height: 0

            text: qsTr("Activate card")
            icon.source: "qrc:/images/private/resources/Interface_icons/Activate.svg"
            onClicked:  () => {
                            privateRoot.activityCard();
                        }
        }

        MenuItem {
            id: ganeralMenuItem

            text: (root.backSide)? qsTr("Hide statistics"): qsTr("Statistics")
            icon.source: "qrc:/images/private/resources/Interface_icons/statistic.svg"
            onClicked: (side) => {
                           privateRoot.showStatistickAction();
                       }
        }

    }

    CMenu {
        id: customisationMenu

        MenuItem {

            text: qsTr("Background color")
            icon.source: "qrc:/images/private/resources/Interface_icons/background_color.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultColor);

                       }

        }

        MenuItem {

            text: qsTr("Foreground color")
            icon.source: "qrc:/images/private/resources/Interface_icons/main_color.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultColorFont);

                       }

        }

        MenuItem {

            text: qsTr("Background image")
            icon.source: "qrc:/images/private/resources/Interface_icons/background_image.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(selectImage);

                       }

        }

        MenuItem {

            text: qsTr("Card logo")
            icon.source: "qrc:/images/private/resources/Interface_icons/logo.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultLogos);

                       }

        }

        MenuItem {

            text: qsTr("Card seal")
            icon.source: "qrc:/images/private/resources/Interface_icons/seal.svg"
            onClicked: () => {
                           activityProcessor.newActivityFromComponent(defaultSeels);
                       }

        }

    }
}
