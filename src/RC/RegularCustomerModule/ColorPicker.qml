import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts


Dialog {
    id: root
    property alias color: preview.color
    RowLayout {

        Rectangle {
            id: preview
            color:  Qt.rgba(red.value, green.value,  blue.value);
            radius: 4
            Layout.preferredWidth:  parent.width / 5
            Layout.fillHeight: true

        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: red
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Red

                }

                TextField {
                    text: red.value.toFixed(4)

                    validator: DoubleValidator {
                        bottom: red.from;
                        top: red.to;
                    }

                    onEditingFinished: {
                        red.value = text
                    }

                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: green
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Green
                }

                TextField {
                    text: green.value.toFixed(4)
                    validator: DoubleValidator {
                        bottom: green.from;
                        top: green.to;
                    }

                    onEditingFinished: {
                        green.value = text
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Slider {
                    id: blue
                    from: 0;
                    to: 1;
                    value: 0
                    stepSize: 0.001
                    Material.accent : Material.Blue

                }

                TextField {
                    text: blue.value.toFixed(4)
                    validator: DoubleValidator {
                        bottom: blue.from;
                        top: blue.to;
                    }

                    onEditingFinished: {
                        blue.value = text
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

    anchors.centerIn: parent

    standardButtons: Dialog.Ok | Dialog.Close
}
