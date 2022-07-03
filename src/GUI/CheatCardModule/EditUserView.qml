import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import com.scythestudio.scodes 1.0

Frame {
    id: root
    property var model: null
    property var userModel: (model)? model.currentUser : null
    property bool fBillingAwailable: mainModel && mainModel.fBillingAwailable()
    property bool editable: true
    property int  maximuWidth: -1

    contentItem:
        ColumnLayout {

            spacing: 0

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: maximuWidth

                horizontalAlignment: Label.AlignHCenter
                text: (root.userModel)? qsTr("About your profile") : qsTr("No user selected")
                wrapMode: Label.WordWrap
                font.bold: true
                font.pointSize: 14
                visible: editable

                padding: 20
            }

            GridLayout {
                Layout.maximumWidth: maximuWidth

                rows: 2
                columns: 2

                Label {
                    text: qsTr("User Id:");
                }

                Label {
                    text: (root.userModel)? root.userModel.name: ""
                    horizontalAlignment: Label.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true

                }

                Label {
                    text: qsTr("Its my name:");

                }

                TextField {
                    id: nameEditor
                    text: (root.userModel)? root.userModel.name: ""
                    horizontalAlignment: Label.AlignHCenter
                    Layout.fillWidth: true

                    readOnly: !root.editable

                    onEditingFinished: () => {
                                           if (!root.userModel && nameEditor.text.length)
                                           return

                                           root.userModel.name = nameEditor.text
                                       }
                }
            }

            Button {
                id: becomeaseller
                text: qsTr("Start integration with business");
                Layout.alignment: Qt.AlignHCenter
                visible: Boolean(root.userModel && !root.userModel.fSaller) && fBillingAwailable

                onClicked: {
                    if (root.userModel) {
                        root.userModel.becomeSellerRequest();
                    }
                }
            }

            Switch {

                checked: Boolean(root.model && root.model.mode)
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Work mode");
                onPositionChanged: () => {
                                       if (root.model) {
                                           root.model.mode = position;
                                       }
                                   }

                visible: !becomeaseller.visible && fBillingAwailable

            }

            Button {
                id: showWorkers
                text: qsTr("Workers management");
                Layout.alignment: Qt.AlignHCenter
                visible:!becomeaseller.visible && fBillingAwailable && root.model.mode

                onClicked: {
                    if (mainModel) {
                        activityProcessor.newActivity("qrc:/CheatCardModule/PermissionsView.qml",
                                                      mainModel.permisionsModel);
                        userPanel.close()
                    }
                }
            }

            Item {
                id: helpMsgWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: implicitHeight

                Layout.minimumHeight:0

                implicitWidth: 0x0
                implicitHeight: helpMsg.implicitHeight

                visible: !Boolean(root.model && root.model.mode)

                clip: true
                Label {
                    id: helpMsg
                    text: qsTr("To receive a card or a seal, as well as to receive an already accumulated bonus, show this Qr code to the seller.");
                    wrapMode: Label.WordWrap
                    elide: Text.ElideRight
                    width: parent.width
                    height: parent.height
                }
            }


            QrCodeControl {

                id: qrBox
                implicitWidth: 0x0
                implicitHeight: 0x0

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumHeight: 100

                visible: !Boolean(root.model && root.model.mode)

                fileName: "currentuserqrcode";
                inputText: (userModel)? (userModel.sessionCode): ""


                MouseArea {
                    anchors.fill: parent

                    onReleased: {
                        activityProcessor.newActivity("qrc:/CheatCardModule/QrCodeView.qml",
                                                      qrBox.qrCodeFilePath);
                        userPanel.close()
                    }
                }
            }
            AdditionalInformationForSeller {
                implicitWidth: 0x0
                id: sellerBox
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.alignment: Qt.AlignHCenter
                visible: Boolean(root.model && root.model.mode)
            }

        }

}
