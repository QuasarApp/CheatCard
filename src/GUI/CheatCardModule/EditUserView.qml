import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

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
                if (root.userModel) {
                    root.userModel.becomeSellerRequest();
                }
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
                anchors.centerIn: parent
                height: Math.min(parent.height, parent.width)
                width: Math.min(parent.height, parent.width)
                fillMode: Image.PreserveAspectFit
                SBarcodeGenerator {
                    id: generator
                    fileName: "currentuserqrcode";
                    inputText: (userModel)? (userModel.sessionCode): ""
                    height: parent.height
                    width:  parent.width
                    margin: 0
                    onInputTextChanged: {
                        if (inputText.length)
                            process(inputText);
                    }
                }
                visible: !Boolean(root.model && root.model.mode)
                source: "file:/" + generator.filePath
                layer.enabled: true
                layer.effect: ShaderColorOverlay {
                    color: Material.primary
                    fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
                }

            }

            MouseArea {
                anchors.fill: parent

                onReleased: {
                    activityProcessor.newActivity("qrc:/CheatCardModule/QrCodeView.qml",
                                                  generator.filePath);
                    userPanel.close()
                }
            }
        }


        Item {
            Layout.fillHeight: true
        }
    }

}
