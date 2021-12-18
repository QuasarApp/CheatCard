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
    signal importFinished()

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

                            if (root.model)
                                root.model.handleDecodeFinished(data);

                            privateRoot.importDataFinished(data)

                        }

            onVisibleChanged: {
                if(!visible)
                    destroy()
            }
        }
    }


    FileDialog {
        id: fromFile
        folder: shortcuts.documents
        nameFilters: [qsTr("codes") + " (*.png)"]

        selectFolder: false
        selectMultiple: false
        selectExisting: true
        title: qsTr("Select your backup qr code")
        onSelectionAccepted: {
            if (root.model)
                root.model.processQrCode(fileUrl);
        }
    }

    Connections {
        target: root.model

        function onDecodeFinished(captured) {
            privateRoot.importDataFinished(captured)
        }
    }

    Item {
        id: privateRoot

        function importDataFinished (data) {
            activityProcessor.popItem();
        }

    }
}
