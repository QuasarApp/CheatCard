//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import com.scythestudio.scodes 1.0

import "Style"

CPage {
    id: root;
    title: qsTr("Import your cadrs data")

    property var model: null

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


        Button {
            text: qsTr("Import from qr code scanner");
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                activityProcessor.newActivityFromComponent(scaner);
            }
        }

        Button {
            text: qsTr("Import from file with qr code");
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                fromFile.open()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Component {
        id: scaner
        QrScanner {
            id:qrScaner
            onCaptured: (data) => {
                            if (!root.model) {
                                return
                            }
                            root.model.importUser(data);
                            activityProcessor.popItem();

                        }

            onVisibleChanged: {
                if(!visible)
                    destroy()
            }
        }
    }


    FileDialog {
        id: fromFile
        folder: (root.model)? "file://" + root.model.userBackUpPath() : ""
        selectFolder: false
        selectMultiple: false
        selectExisting: true
        title: qsTr("Select your backup qr code")
        nameFilters: ["*.qrrc*"]
        onSelectionAccepted: {

            barcodeFilter.process(fileUrl)
        }
    }

    SBarcodeFilter {
        id: barcodeFilter


        onCapturedChanged: (captured) => {
                               mainModel.importUser(captured);
                           }
    }
}
