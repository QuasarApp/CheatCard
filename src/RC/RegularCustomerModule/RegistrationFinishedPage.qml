import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root

    signal finished();

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: qsTr("Registaration passed!!!")
        font.bold: true
    }

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: qsTr("The registration of the new user finished successful. Now you can use you card.")
                wrapMode: Label.WordWrap
            }

            Button {
                text: qsTr("Go to use")
                Layout.alignment: Qt.AlignHCenter

                onClicked: () => {
                               root.finished()
                           }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
