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
    title: qsTr("Export user key")
    implicitHeight: 0x0
    implicitWidth: 0x0

    property var model: null

    contentItem: ColumnLayout {

        Item {
            Layout.fillHeight: true
        }

        Label {
            text: qsTr("**For what this use**")
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: TextInput.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            textFormat: Text.MarkdownText

        }

        Label {
            text: qsTr("Some times you can drop your application data or your device. Therefore you can create a backup qr code that can recovery your application data after dropping")
            Layout.fillWidth: true
            wrapMode: Label.WordWrap
            horizontalAlignment: TextInput.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            textFormat: Text.MarkdownText

        }

        Image {
            id: imgQr
            Layout.preferredHeight: Math.min(root.height * 0.7, root.width * 0.7)
            Layout.preferredWidth: height
            Layout.alignment: Qt.AlignHCenter

            fillMode: Image.PreserveAspectFit
            SBarcodeGenerator {
                id: generator
                fileName: "CheatCard-" + ((root.model)? root.model.name: "") + ".qrrc";
                inputText: (root.model)? root.model.userBackUpData(): "Sorry"
                height: parent.height
                width:  parent.width
                margin: 0
                onInputTextChanged: {
                    if (inputText.length)
                        process(inputText);
                    imgQr.qrIndex++

                }

                onWidthChanged: {
                    if (inputText.length)
                        process(inputText);
                    imgQr.qrIndex++
                }

            }
            property int qrIndex: 0

            source: "image://cards/file:" + generator.filePath + ":" + qrIndex
            layer.enabled: true
            layer.effect: ShaderColorOverlay {
                color: Material.primary
                fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
            }

        }
        Button {
            text: qsTr("Save and open in files");
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                generator.saveImage();
                notificationService.setNotify(qsTr("Done"), qsTr("Backup code exported successful. The Backup Code Available in your documents' folder. See <b>%0</b> path").arg(root.model.userBackUpPath()));
            }

        }

        Item {
            Layout.fillHeight: true
        }
    }
}
