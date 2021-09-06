import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property var model: null

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: list
            model: root.model
            currentIndex: 0;
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component {
                id: delegateItem

                EditCardView {

                    model: card
                    height: width * 0.5
                    width: list.width
                    clip: true
//                    opacity: (list.currentIndex == index)? 1: 0.5
                }
            }

            delegate: delegateItem
        }

        Button {
            text: qsTr("Add card")
            Layout.alignment: Qt.AlignHCenter

            onClicked: () => {
                           inputName.open()
                       }
        }

    }

    Dialog {
        id: inputName;

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        TextField {
            id: input
            placeholderText: qsTr("Enter new nama");
            Layout.alignment: Qt.AlignHCenter

        }

        Component.onCompleted: {
            var applayButton = standardButton(Dialog.Ok)

            applayButton.enabled = input.length
            input.onTextChanged.connect(()=> {applayButton.enabled = input.length})
        }

        onAccepted: () => {
                        if (!root.model) {
                            return;
                        }

                        root.model.addCard(input.text)
                    }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }
}
