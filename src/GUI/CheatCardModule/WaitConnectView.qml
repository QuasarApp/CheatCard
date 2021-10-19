//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property var model: null

    implicitHeight: 0x0    // disable resize by content

    contentItem:
        ColumnLayout {

        Item {
            Layout.fillHeight: true
        }

        EditCardView {

            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: Math.min(root.width, root.height)
            Layout.preferredHeight: width * 0.75
            purchasesNumber: purchaseInput.value
            editable: false
            model: (root.model)? root.model.card : null
        }

        RowLayout {

            Layout.alignment: Qt.AlignHCenter

            SpinBox {
                id: purchaseInput
                Layout.alignment: Qt.AlignHCenter

                from: 1
                to: 10

                stepSize: 1

                onValueChanged: () => {
                                    if (root.model) {
                                        root.model.purchaseCount = value
                                    }
                                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                id: start
                text: qsTr("Seal");

                onClicked:  () => {
                                if (root.model) {
                                    activityProcessor.newActivityFromComponent(scaner);
                                }
                            }
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
                            root.model.extraData = data;
                            root.model.begin();
                            activityProcessor.popItem();

                        }
            onVisibleChanged: {
                if(!visible)
                    destroy()
            }
        }
    }
}
