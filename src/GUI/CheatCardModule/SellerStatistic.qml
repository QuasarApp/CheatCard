import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root;

    property var model: null

    implicitHeight: 0x0
    implicitWidth: 0x0

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

                delegate: Item {
                    property string dataText: display
                    ToolButton {
                        text: dataText
                        anchors.fill: parent

                        onReleased: {
                            if (root.model) {
                                root.model.sortView(column)
                            }
                        }
                    }
                }
            }

            contentItem: TableView {
                id: tableView
                model: (root.model)? root.model.proxy(): null
                clip: true

                ScrollBar.vertical: ScrollBar { }

                property var columnWidths: [-80, 80, 90, 90, 200]
                columnWidthProvider: function (column) {

                    const width = columnWidths[column];
                    if (!width) {
                        let sum = 0;
                        columnWidths.forEach((item)=> {
                                                 if (item > 0)
                                                    sum += item;
                                             })

                        return Math.max(tableView.width - sum, Math.abs(width))
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
                        readOnly: true;
                        anchors.fill: parent
                    }
                }
            }
        }

    }
}
