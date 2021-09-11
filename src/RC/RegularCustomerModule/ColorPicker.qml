import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15


Dialog {
    id: root
    property alias color: preview.color
    ColumnLayout {

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
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Red
                    Layout.fillWidth: true

                }

                TextField {
                    text: red.value.toFixed(4)

                    validator: DoubleValidator {
                        bottom: red.from;
                        top: red.to;
                    }
                    horizontalAlignment: Text.AlignHCenter
                    onEditingFinished: {
                        red.value = text
                    }

                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: green
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Green
                    Layout.fillWidth: true

                }

                TextField {
                    text: green.value.toFixed(4)
                    validator: DoubleValidator {
                        bottom: green.from;
                        top: green.to;
                    }
                    horizontalAlignment: Text.AlignHCenter

                    onEditingFinished: {
                        green.value = text
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: blue
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Blue
                    Layout.fillWidth: true

                }

                TextField {
                    text: blue.value.toFixed(4)
                    validator: DoubleValidator {
                        bottom: blue.from;
                        top: blue.to;
                    }
                    horizontalAlignment: Text.AlignHCenter

                    onEditingFinished: {
                        blue.value = text
                    }
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Random collor")

                Component.onCompleted: () => {
                                           blue.value = Math.random()
                                           red.value = Math.random()
                                           green.value = Math.random()
                                       }

                onClicked: () => {
                               blue.value = Math.random()
                               red.value = Math.random()
                               green.value = Math.random()

                           }
            }
        }
    }

    anchors.centerIn: parent

    standardButtons: Dialog.Ok | Dialog.Close
}
