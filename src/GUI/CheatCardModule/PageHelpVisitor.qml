//#
//# Copyright (C) 2018 - 2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QuasarAppCreditsModule 1.0


Credits {
    property var model: null
    property string toolBarTitle: qsTr("Help page")

    state: "about"
    listCustomInfo: [
        {
            text: qsTr("### How to use?"),
            align: Label.AlignHCenter,
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;The algorithm of actions for the buyer works according to the system:"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Came to the establishment."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Made an order."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Showed the QR code in the application at the checkout."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Receive seal to card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Repeated this procedure several times and received the bonus set by the seller."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;In order to open the QR code, click on the item in the upper left corner of the screen. "),
            align: Label.AlignLeft
        },
        {
            text: ("<img src=\"qrc:/private/resources/help/Help1.jpg\" alt=\"tt\" width=\"100\"/>"),
            align: Label.AlignHCenter,
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;A sidebar will open at the bottom of which there will be a QR code, you can click on it to enlarge it."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;After receiving the card, you can view additional information about the card by clicking on it."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        }
    ]

}
