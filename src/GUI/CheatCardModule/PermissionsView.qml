//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import "Style"

Page {

    Layout.fillWidth: true
    Layout.fillHeight: true

    header: HorizontalHeaderView {
        id: horizontalHeader
        syncView: tableView
        clip: true

        delegate: Item {
            property string dataText: display
            ToolButton {
                text: dataText
                anchors.fill: parent
                implicitHeight: 0x0
                implicitWidth: 0x0
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

        property var columnWidths: [-120, 80, 90, 100, 90, 200, 100]
        property int selectedRow: 0

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

            property bool fSelect: row === tableView.selectedRow

            onFSelectChanged: {
                if (root.model && fSelect)
                    root.model.chouseRow(row);
            }

            color: (fSelect)? Material.accent:
                              (row % 2 === 0) ? "#55888888":
                                                "#11888888"


            Label {
                text: display
                wrapMode: Label.WordWrap
                anchors.fill: parent
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
            }

            MouseArea {
                onClicked: tableView.selectedRow = row
                anchors.fill: parent
            }
        }
    }
}
