
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Page {
    id: root;
    property var model: null
    property string currentSelectedItem: ""

    signal selected(var itemId)

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: title
        font.bold: true
        visible: text.length
    }

    ColumnLayout {
        anchors.fill: parent
        GridView {
            id: gridView
            model: root.model
            highlightFollowsCurrentItem: true
            ScrollBar.vertical: ScrollBar {}
            clip: true

            Layout.fillHeight: true
            Layout.fillWidth: true

            cellHeight: 110
            cellWidth:110

            Component {
                id: gridDelegate

                Rectangle {
                    property bool current: display === currentSelectedItem
                    color: (current)? Material.accent: "#777777"
                    radius: 4
                    width: 100
                    height: 100

                    Image {
                        Layout.fillWidth: true

                        anchors.fill: parent

                        fillMode: Image.PreserveAspectFit
                        source: display
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: () => {
                                       currentSelectedItem = display
                                   }
                    }
                }
            }

            delegate: gridDelegate
        }


        Button {
            text: qsTr("Add");
            Layout.alignment: Qt.AlignHCenter
            Component {
                id: filedialog


                FileDialog {
                    id: fileDialogView
                    property var model: null

                    title: qsTr("Please choose a new picture")
                    folder: shortcuts.home
                    onAccepted: {

                        if (!fileDialogView.model)
                            return;

                        fileDialogView.model.addCustomItem(fileDialogView.fileUrls)
                        activityProcessor.popItem();
                    }

                    Component.onCompleted: visible = true
                }
            }

            onClicked: {
                if (!root.model) {
                    return;
                }

                activityProcessor.newActivity(filedialog,
                                              root.model);
            }
        }
    }
}
