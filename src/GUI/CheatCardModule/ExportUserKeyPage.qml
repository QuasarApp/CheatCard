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

import "Style"

CPage {

    id : root
    title: qsTr("Export user key")
    implicitHeight: 0x0
    implicitWidth: 0x0

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
                fileName: "recovery";
                inputText: "recovery"
                height: parent.height
                width:  parent.width
                margin: 0
                onInputTextChanged: {
                    if (inputText.length)
                        process(inputText);
                }

                onWidthChanged: {
                    if (inputText.length)
                        process(inputText);
                }

            }
            source: "file:/" + generator.filePath
            layer.enabled: true
            layer.effect: ShaderColorOverlay {
                color: Material.primary
                fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
            }

        }
        Button {
            text: qsTr("Export to file");
            Layout.alignment: Qt.AlignHCenter

        }

        Item {
            Layout.fillHeight: true
        }
    }
}
