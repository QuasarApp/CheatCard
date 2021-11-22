//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "Style"

CPage {

    title: qsTr("Recovery your cadrs data")

    contentItem: ColumnLayout {

        Item {
            Layout.fillHeight: true
        }

        Label {
            text: qsTr("**Attention** : This operation redefine your current application data (Cards, seals, users data). All current data will be removed.")
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: TextInput.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            textFormat: Text.MarkdownText
        }

        CTextField {
            placeholderText: qsTr("Enter your recovery word");
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: qsTr("Next");
        }

        Item {
            Layout.fillHeight: true
        }
    }

}
