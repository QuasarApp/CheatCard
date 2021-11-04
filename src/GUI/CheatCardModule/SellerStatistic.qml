import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root;

    property var model: null

    contentItem: GridLayout {

        property bool fHorisontal: root.width > root.height

        columns: (fHorisontal)? 2 : 1
        rows: (fHorisontal)? 1: 2

        EditCardView {
            Layout.preferredWidth: Math.min(root.width, root.height)
            Layout.preferredHeight: width * 0.7
            model: (root.model)? root.model.currentCard: null
            editable: false
        }

        Page {

            Layout.fillWidth: true
            Layout.fillHeight: true

            header: HorizontalHeaderView {
                id: horizontalHeader
                syncView: tableView

                delegate: Rectangle {
                    color: "#00000000"
                    Label {
                        text: display
                        horizontalAlignment:  Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        anchors.fill: parent
                    }
                }
            }

            contentItem: TableView {
                id: tableView
                model: (root.model)? root.model.proxy(): null
                clip: true

                property var columnWidths: [0, 80, 80]
                columnWidthProvider: function (column) {

                    const width = columnWidths[column];
                    if (!width) {
                        let sum = 0;
                        columnWidths.forEach((item)=> {
                                                 sum += item;
                                             })

                        return tableView.width - sum
                    }

                    return width;
                }


                delegate: Rectangle {
                    color: "#00000000"
                    implicitWidth: root.width * 0.3
                    implicitHeight: 50

                    TextField {
                        text: display
                        horizontalAlignment:  Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        wrapMode: Text.WordWrap

                        anchors.fill: parent
                    }
                }
            }
        }

    }
}
