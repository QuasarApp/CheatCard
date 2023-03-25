
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "Style"

CPage {
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
                    property bool isAdd: display === "add";
                    property bool isClear: display === "clear";
                    property bool isButton: isAdd || isClear

                    color: (current)? Material.accent: "#777777"
                    radius: 4
                    width: 100
                    height: 100

                    Image {
                        Layout.fillWidth: true
                        opacity: (current)? 0.75 : 1
                        anchors.fill: parent
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        source: (isButton)? "" : display
                        visible: !isButton
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: () => {
                                       selected(display)
                                   }
                    }

                    ToolButton {
                        text: (isAdd)? qsTr("Gallery"): qsTr("Clear");
                        visible: isButton
                        anchors.fill: parent

                        onClicked: {
                            if (!root.model) {
                                return;
                            }

                            if (isAdd) {
                                fileDialogView.open();
                                return;
                            }

                            if (isClear) {
                                selected("")
                                return;
                            }
                        }


                        /// to do
                        /// fix bug Unable to assign RC::CardModel to QString.
                        /// this bug occurred sometime and i don't understand why this happened. i thin this is bug of qt library
                        FileDialog {
                            id: fileDialogView
                            nameFilters: [qsTr("Images") + " (*.jpg *.png)"]
                            title: qsTr("Do you want to put your picture? You can choose but the picture must be in png format!")
                            folder: shortcuts.home
                            onAccepted: {

                                if (!root.model)
                                    return;

                                root.selected(fileDialogView.fileUrls[0])
                            }

                            onRejected: {
                            }

                        }
                    }
                }
            }

            delegate: gridDelegate
        }
    }
}
