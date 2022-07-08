//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import com.scythestudio.scodes 1.0
import QtQuick.Dialogs 1.3
import NotifyModule 1.0

import "Style"

CPage {

    id : root
    title: qsTr("Export user account")
    implicitHeight: 0x0
    implicitWidth: 0x0

    property var model: null
    property bool fExport: false

    contentItem: ColumnLayout {

        Label {
            text: (fExport)? qsTr("**For what this use**"):
                             qsTr("**Sharing access to your account**")
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: TextInput.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            textFormat: Label.MarkdownText
            visible: !fExport;
        }

        Label {
            text: qsTr("Some times you can drop your application data or your device. Therefore you can create a backup qr code that can recovery your application data after dropping")

            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignJustify
            Layout.alignment: Qt.AlignHCenter
            textFormat: Label.MarkdownText
            visible: !fExport;
        }

        Label {
            text: qsTr("Scan This Qr code using the 'import user' button on the bottom of a user's list page on another device to export this user account to another device.")
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignJustify
            Layout.alignment: Qt.AlignHCenter
            textFormat: Label.MarkdownText
            visible: fExport;

        }

        QrCodeControl {
            id: imgQr


            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: Math.min(root.height * 0.7, root.width * 0.7)
            Layout.maximumHeight: Math.min(root.height * 0.7, root.width * 0.7)

            Layout.alignment: Qt.AlignHCenter
            fileName: "CheatCard-" + ((root.model)? root.model.name: "") + ".qrrc";
            inputText: (root.model)? root.model.userBackUpData(): ""
        }

        Button {
            text: qsTr("Save and open in files");
            Layout.alignment: Qt.AlignHCenter
            visible: !fExport;

            onClicked: {
                imgQr.saveImage();
                notificationService.setNotify(qsTr("Done"), qsTr("Backup code exported successful. The Backup Code Available in your documents' folder. See <b>%0</b> path").arg(root.model.userBackUpPath()));
            }

        }
    }
}
