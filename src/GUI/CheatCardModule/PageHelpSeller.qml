//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
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
            text: qsTr("### Create a card for your establishment"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* In order to create a map, click the \"Add Map\" button at the bottom of the screen."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* An empty card will be created."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* In order to fill it out, click the \"Edit\" button on the card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("<img src=\"qrc:/private/resources/help/Help3.jpg\" alt=\"tt\" width=\"300\"/>"),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("* Enter the contact and other details of your establishment. In the item \"Bonus name\" it is necessary to indicate what the client will receive in the form of a bonus (product / service / discount / etc.)."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Indicate how many purchases the client must make to receive the bonus you previously specified."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Customize the design of the card by clicking on this element at the bottom left of the card."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("<img src=\"qrc:/private/resources/help/Help2.jpg\" alt=\"tt\" width=\"300\"/>"),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("* A menu with design settings (background and text colors, logo, as well as printing) will open"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Optionally upload your logo or card print background design by clicking the add button in the appropriate sections, the image must be in png format."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* After completing all the settings, click the \"Save\" button which is located at the bottom of the map."),
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
            text: qsTr("### Issue a card to a client:"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* The created map will be displayed on the main screen of the application."),
            align: Label.AlignLeft
        },

        {
            text: qsTr("* In order to issue the card to the client, press and hold on it."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* A screen will open where you can specify how many stamps to put to the client, then click \"print\"."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Scan the visitor's QR code."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* The client will have a card with the number of stamps that you specified."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Repeat this algorithm in order to put in the future seals to the client when purchasing a product."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("### Issue bonus:"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* When the client has collected a sufficient number of stamps, you can give him a bonus, the application itself will notify you that the client has made a sufficient number of purchases."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* On the bonus screen, you can click the \"Issue\" button, or if the client can ask to save this bonus, then just click the back button."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Bonuses can be accumulated by the client, if the client has accumulated several bonuses, then on the issuance screen you can choose how many bonuses to issue, after which they will be written off from the category of available ones and will be transferred to the category received on the client's card."),
            align: Label.AlignLeft
        }
    ]

}
