import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "Style"

CPage {
    property alias color: preview.color
    property color currentlyColor: Material.primary
    implicitHeight: 0x0

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: preview
            color:  Qt.rgba(red.value, green.value,  blue.value);
            radius: 4
            Layout.preferredHeight:  parent.height / 8
            Layout.fillWidth: true

        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: red
                    from: 0
                    to: 1
                    stepSize: 0.001
                    value: currentlyColor.r
                    Material.accent : Material.Red
                    Layout.fillWidth: true

                }

                TextField {
                    text: red.value.toFixed(4)
                    horizontalAlignment: Label.AlignHCenter

                    onEditingFinished: {
                        red.value = Number(text)
                    }

                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: green
                    from: 0
                    to: 1
                    stepSize: 0.001
                    value: currentlyColor.g
                    Material.accent : Material.Green
                    Layout.fillWidth: true

                }

                TextField {
                    text: green.value.toFixed(4)
                    horizontalAlignment: Label.AlignHCenter

                    onEditingFinished: {
                        green.value = Number(text)
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: blue
                    from: 0
                    to: 1
                    stepSize: 0.001
                    value: currentlyColor.b
                    Material.accent : Material.Blue
                    Layout.fillWidth: true

                }

                TextField {
                    text: blue.value.toFixed(4)
                    horizontalAlignment: Label.AlignHCenter

                    onEditingFinished: {
                        blue.value = Number(text)
                    }
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Random collor")

                onClicked: () => {
                               blue.value = Math.random()
                               red.value = Math.random()
                               green.value = Math.random()

                           }
            }
        }
    }

}

