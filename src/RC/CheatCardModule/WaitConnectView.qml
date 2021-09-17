//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property var model: null
    signal complete();

    contentItem:
        ColumnLayout {
        EditCardView {
            Layout.fillWidth: true
            Layout.preferredHeight: width * 0.7
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
                                    root.complete();
                                }
                            }
            }
        }

        ProgressBar {
            Layout.fillWidth: true
            from: 0
            to: 10000
            value: (root.model)? root.model.timeOut : 0
        }
    }
}
