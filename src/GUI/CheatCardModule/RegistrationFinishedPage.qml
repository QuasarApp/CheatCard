import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "Style"

CPage {
    id: root

    signal finished(var backupData);

    title: qsTr("Done!")

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent
            spacing: 20
            Item {
                Layout.fillHeight: true
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: qsTr("All Done! <b>But If you drop your application data then all your cards will be removed</b>. For save it self of this situations we recommends create a back up your master key.")
                wrapMode: Label.WordWrap
                horizontalAlignment: TextInput.AlignHCenter
            }

            RowLayout {
                Layout.preferredWidth: parent.width
                CheckBox {
                    id: restore
                    Layout.alignment: Qt.AlignCenter

                }

                Label {
                    Layout.fillWidth: true
                    wrapMode: Label.WordWrap
                    horizontalAlignment: TextInput.AlignLeft
                    text: qsTr("Create a back up of master key for save data of my account.")
                }
            }

            Button {
                text: qsTr("Go to use")
                Layout.alignment: Qt.AlignHCenter

                onClicked: () => {
                               root.finished(restore.checked)
                           }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
