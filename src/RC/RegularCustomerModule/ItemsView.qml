
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

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

    contentItem: GridView {
        id: gridView
        model: root.model
        highlightFollowsCurrentItem: true
        ScrollBar.vertical: ScrollBar {}

        Component {
            id: gridDelegate
            Rectangle {
                property bool current: display === currentSelectedItem
                color: (current)? Material.accent: "#00000000"
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
}
