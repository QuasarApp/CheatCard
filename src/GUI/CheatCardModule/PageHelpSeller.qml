//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QuasarAppCreditsModule 1.0


Credits {
    state: "about"
    iconWidth: 150
    listCustomInfo: [
        {
            text: qsTr("### Guide to using Cheat Cards for merchants"),
            align: Label.AlignHCenter,
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;In this section, you can find information about using the Cheat card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("### How to use?"),
            align: Label.AlignHCenter,
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;To implement the loyalty program of your institution using a cheat card, you need to do the following:"),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("* Create a card for your establishment"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("<img src=\"qrc:/private/resources/help/Help3.jpg\" alt=\"tt\" width=\"300\"/>"),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;In order to create a map, click the \"Add Map\" button at the bottom of the screen. After that, an empty card will be created, in order to fill it out, press the \"Edit\" button on the card. Now you can enter the contact and other data of your establishment, it is not necessary to fill in all the fields, except for those highlighted in red \"Card name\" and \"Bonus name\". In the item \"Bonus name\" it is necessary to indicate what the client will receive in the form of a bonus (product / service / discount / etc.). Next, indicate how many purchases the client must make to receive the bonus you previously specified."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;After that, you can customize the design of the card by clicking on this element at the bottom left of the card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("<img src=\"qrc:/private/resources/help/Help2.jpg\" alt=\"tt\" width=\"300\"/>"),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;A menu will open in which you can customize the background and text colors, logo, as well as printing. You can upload your logo or card print background design by clicking the add button in the appropriate sections, the image must be in png format."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;After completing all the settings, click the \"Save\" button located at the bottom of the map."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;**IMPORTANT:** After you put at least one seal on the customer, the card cannot be edited. Instead, you can create a new map based on the old one. all customer data and their statistics will be automatically transferred to the corrected card. Instead, you can create a new map based on the old one. all customer data and their statistics will be automatically transferred to the corrected card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("* Issue a card to a client"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;After you have created and configured the map, it will be displayed on the main screen of the application. In order to issue a card to a client, you need to press and hold on it, after which a screen will open where you can specify how many stamps to put to the client, then press print. Then scan the visitor's QR code. After that, the client will have a card with the number of seals that you specified. In the same way, you can put stamps to the client in the future when buying a product."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("* Issue a bonus"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;When the client has collected a sufficient number of stamps, you can give him a bonus, the application itself will notify you that the client has made a sufficient number of purchases. On the bonus issue screen, you can click the 'Issue' button, or if the client can ask to save this bonus, then just click the back button. Bonuses can be accumulated by the client, if the client has accumulated several bonuses, then on the issuance screen you can choose how many bonuses to issue, after which they will be written off from the category of available ones and transferred to the category received on the client's card."),
            align: Label.AlignLeft
        }
    ]

}
