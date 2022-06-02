import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "Style"

Control {

    id: root;
    contentItem: Flickable {
        id: flickable
        ScrollBar.vertical: ScrollBar {}
        contentHeight: listContacts.height
        clip: true
        ColumnLayout {
            id: listContacts
            width: flickable.width

            Label {
                text: qsTr("Have any questions?");
                Layout.fillWidth: true
                wrapMode: Label.WordWrap
                font.bold: true
            }

            Label {
                text: qsTr("* You can lern more about using cheatcard in the help page or introduction video:");

                Layout.fillWidth: true
                wrapMode: Label.WordWrap

            }

            Button {
                text: qsTr("Open the help page")
                Layout.fillWidth: true

                onClicked:  {
                    backButton.clicked()

                    activityProcessor.newActivity(
                                "qrc:/CheatCardModule/PageHelpSeller.qml");
                }

            }

            Button {
                text: qsTr("See the help video")
                Layout.fillWidth: true
                onClicked:  {
                    onClicked: Qt.openUrlExternally(
                                   "https://www.youtube.com/watch?v=JWdokWEAGEg&list=PLK83dOS43ZMukJ8308eBVw8qcYgpguiW_")
                }
            }

            Label {
                text: qsTr("* You can additionals seller resources:");
                Layout.fillWidth: true
                wrapMode: Label.WordWrap

            }

            Button {
                text: qsTr("Get CheatCard Banners")
                Layout.fillWidth: true

                onClicked:  {
                    onClicked: Qt.openUrlExternally(
                                   "https://quasarapp.ddns.net:444/index.php/s/yyQfWNCcz874KcP?path=%2FBanners")

                }
            }

            Label {
                text: qsTr("* Or You can contact with developers here:");
                Layout.fillWidth: true
                wrapMode: Label.WordWrap

            }

            Contacts {
                Layout.fillWidth: true

                imageSize: parent.width / 5

            }

        }
    }
}
