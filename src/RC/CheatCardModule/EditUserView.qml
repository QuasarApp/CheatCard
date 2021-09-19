import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property var model: null
    property bool editable: true

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: (root.model)? qsTr("Editing : ") + root.model.name : qsTr("No user selected")
        font.bold: true
        font.pointSize: 14
        visible: editable

        padding: 20
    }

    contentItem: GridLayout {

        columns: 2
        rows: 4
        rowSpacing: 10
        columnSpacing: 10
        Label {
            text: qsTr("User Id:");
        }

        Label {
            text: (root.model)? root.model.name: ""
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: qsTr("Visible user name:");
        }

        TextField {
            id: nameEditor
            text: (root.model)? root.model.name: ""
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true

            readOnly: !root.editable

            onEditingFinished: () => {
                if (!root.model && nameEditor.text.length)
                    return

                root.model.name = nameEditor.text
            }
        }

        Label {
            text: qsTr("This user is not saller");
            visible: (root.model) && !root.model.fSaller
        }

        Button {
            text: qsTr("Become a Saller");
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 1
            visible: (root.model) && !root.model.fSaller

            onClicked: {
                becomeSallerDialog.open()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Dialog {
        id: becomeSallerDialog

        header: Label {
            horizontalAlignment: Label.AlignHCenter
            text: qsTr("Are you sure you want to become a seller? ");
            font.bold: true
        }

        standardButtons: Dialog.Yes | Dialog.No

        onAccepted: () => {
                        if (root.model) {
                            root.model.fSaller = true;
                        }
                        close();
                    }

        onRejected: close();
    }
}
