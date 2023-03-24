import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
 import QtQuick.Dialogs 1.3
import "Style"

CPage {
    property alias color: preview.color
    property color currentlyColor: Material.primary
    implicitHeight: 0x0

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: preview
            color:  Qt.rgba(red.value / 255, green.value / 255,  blue.value / 255);
            radius: 4
            Layout.preferredHeight:  parent.height / 8
            Layout.fillWidth: true

        }

        GridLayout {

            columns: 2
            rows: 4

            Label {
                text: qsTr("Color values")
                Layout.alignment: Qt.AlignHCenter

                horizontalAlignment: Label.AlignHCenter

            }

            Label {
                horizontalAlignment: Label.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("#HTML")
            }

            Slider {
                id: red
                from: 0
                to: 255
                stepSize: 1
                value: currentlyColor.r * 255
                Material.accent : Material.Red
                Layout.fillWidth: true

            }

            TextField {
                text: red.value.toString(16)
                horizontalAlignment: Label.AlignHCenter

                onEditingFinished: {
                    red.value = parseInt(text, 16)
                }

            }

            Slider {
                id: green
                from: 0
                to: 255
                stepSize: 1
                value: currentlyColor.g * 255
                Material.accent : Material.Green
                Layout.fillWidth: true

            }

            TextField {
                text: green.value.toString(16)
                horizontalAlignment: Label.AlignHCenter

                onEditingFinished: {
                    green.value =  parseInt(text, 16)
                }
            }

            Slider {
                id: blue
                from: 0
                to: 255
                stepSize: 1
                value: currentlyColor.b * 255
                Material.accent : Material.Blue
                Layout.fillWidth: true

            }

            TextField {
                text: blue.value.toString(16)
                horizontalAlignment: Label.AlignHCenter

                onEditingFinished: {
                    blue.value =  parseInt(text, 16)
                }
            }
        }

        Button {
            Layout.alignment: Qt.AlignHCenter

            text: qsTr("Random collor")

            onClicked: () => {
                           blue.value = Math.floor(Math.random() * 255)
                           red.value =  Math.floor(Math.random() * 255)
                           green.value =  Math.floor(Math.random() * 255)

                       }
        }

    }

}

