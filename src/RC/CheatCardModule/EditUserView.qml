import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property var model: null
    property var userModel: (model)? model.currentUser : null

    property bool editable: true

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: (root.userModel)? qsTr("Editing : ") + root.userModel.name : qsTr("No user selected")
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
            text: (root.userModel)? root.userModel.name: ""
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: qsTr("Visible user name:");
        }

        TextField {
            id: nameEditor
            text: (root.userModel)? root.userModel.name: ""
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true

            readOnly: !root.editable

            onEditingFinished: () => {
                if (!root.userModel && nameEditor.text.length)
                    return

                root.userModel.name = nameEditor.text
            }
        }

        Label {
            text: (becomeaseller.visible)?
                      qsTr("This user is not seller"):
                      qsTr("This user is seller");
        }

        Button {
            id: becomeaseller
            text: qsTr("Become a seller");
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 1
            visible: Boolean(root.userModel && !root.userModel.fSaller)

            onClicked: {
                becomeSallerDialog.open()
            }
        }

        Switch {

            checked: Boolean(root.model && root.model.mode)

            text: qsTr("Seller mode");
            onPositionChanged: () => {
                                    if (root.model) {
                                        root.model.mode = position;
                                    }
                               }

            visible: !becomeaseller.visible

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
                        if (root.userModel) {
                            root.userModel.fSaller = true;
                        }
                        close();
                    }

        onRejected: close();
    }
}
