//#
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

Page {
    id: root
    property var model: null

    implicitHeight: 0x0    // disable resize by content

    onModelChanged: () => {
                        if (root.model) {
                            root.model.purchaseCount = purchaseInput.value
                            root.model.allowScreenDim = !visible;

                        }
                    }


    onVisibleChanged: () => {
                          if (root.model)
                          root.model.allowScreenDim = !visible;
                      }

    contentItem:     Control {
        Frame {
            id: waitForCondition
            visible : model && model.waitConfirm
            anchors.fill: parent

            contentItem: ColumnLayout {
                Label {
                    text: qsTr("Please wait for response from server");
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                    running: true;
                }
            }
        }


        ColumnLayout {
            id: waitConnectView
            anchors.fill: parent
            visible : model && !model.waitConfirm
            Item {
                Layout.fillHeight: true
            }

            Label {
                text: qsTr("Select the number of purchases that the customer has made.")

                font.pointSize: 16
                padding: 10
                Layout.fillWidth: true
                wrapMode: Label.WordWrap
                horizontalAlignment: Label.AlignHCenter

            }

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

                    from: 0
                    to: 10
                    value: 0
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
                    text: (purchaseInput.value)? qsTr("Seal"): qsTr("Check");

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
