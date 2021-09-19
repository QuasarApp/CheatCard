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
    signal complete();

    contentItem:
        ColumnLayout {

        Item {
            Layout.fillHeight: true

        }

        EditCardView {
            Layout.fillWidth: true
            Layout.preferredHeight: width * 0.7
            purchasesNumber: purchaseInput.value
            editable: false
            model: (root.model)? root.model.card : null
        }

        RowLayout {

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
                text: qsTr("Seel");

                onClicked:  () => {
                                if (root.model) {
                                    root.model.begin();
                                }
                            }
            }
            visible: !progress.visible
        }

        ProgressBar {

            id: progress;
            Layout.fillWidth: true
            visible: Boolean(root.model && root.model.waitForConnect)
            from: 1
            to: (root.model)? root.model.waitTime : 0
            value: (root.model)? root.model.timeOut : 0

            Behavior on value {
                NumberAnimation {
                    duration: 1001
                }
            }
        }

        Item {
            Layout.fillHeight: true

        }
    }

    Connections {
        target: model
        function onPurchaseTaskCanceled () {
            root.complete();
        }

        function onPurchaseTaskFinished () {
            root.complete();
        }
    }
}
