import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root

    signal finished();

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: qsTr("Done!")
        font.pointSize: 20
        color: "#424242"
        wrapMode: Label.WordWrap

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
                text: qsTr("Now let's move on to using!")
                wrapMode: Label.WordWrap
                horizontalAlignment: TextInput.AlignHCenter
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
