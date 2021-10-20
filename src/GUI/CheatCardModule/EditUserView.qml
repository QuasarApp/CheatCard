import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import com.scythestudio.scodes 1.0

Frame {
    id: root
    property var model: null
    property var userModel: (model)? model.currentUser : null

    property bool editable: true


    contentItem: GridLayout {

        columns: 2
        rows: 5
        rowSpacing: 5
        columnSpacing: 10
        Label {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter

            horizontalAlignment: Label.AlignHCenter
            text: (root.userModel)? qsTr("Profile") : qsTr("No user selected")
            font.bold: true
            font.pointSize: 14
            visible: editable

            padding: 20
        }
        Label {
            text: qsTr("User Id:");
        }

        Label {
            text: (root.userModel)? root.userModel.name: ""
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: qsTr("Its my name:");
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
                      qsTr("Just a customer"):
                      qsTr("Salesman!");
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
            id: qrBox
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2

            Image {
                id: imgQr
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                visible: !Boolean(root.model && root.model.mode)
                SBarcodeGenerator {
                    id: generator
                    fileName: "currentuserqrcode";
                    inputText: (userModel)? (userModel.sessionCode): ""
                    height: Math.min(parent.height, parent.width)
                    width: Math.min(parent.height, parent.width)
                    margin: 0
                    onInputTextChanged: {
                        if (inputText.length)
                            process(inputText);
                    }
                }

                source: "file:/" + generator.filePath

                ColorOverlayUser {
                    src: imgQr
                }

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
            text: qsTr("Oh, do you really want? ");
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
