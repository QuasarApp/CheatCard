//# Copyright (C) 2021-2023 QuasarApp.
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
    title: qsTr("Import your cards data")

    property var model: null
    property bool onlyScaner: false

    signal importFinished()

    contentItem: ColumnLayout {

        Item {
            Layout.fillHeight: true
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
            visible: !onlyScaner;
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
            if (root.model && root.model.processQrCode(fileUrl)) {
                importFinished();
            }
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
            importFinished();
            activityProcessor.popItem();
        }

    }
}
