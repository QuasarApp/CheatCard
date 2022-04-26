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

CPage {
    id: root
    title : qsTr("Outdated!")
    implicitWidth: 0x0
    implicitHeight: 0x0

    contentItem:
        Frame {
        anchors.fill: parent
        contentItem:
            ColumnLayout {

            ToolButton {
                id: editCardBtn
                icon.source: "qrc:/images/private/resources/Interface_icons/outdated.svg"
                icon.color: Material.accent
                icon.height: Math.min(root.width, root.height) * 0.5
                icon.width: Math.min(root.width, root.height) * 0.5
                Layout.alignment: Qt.AlignCenter
            }

            Label {
                Layout.alignment: Qt.AlignCenter
                wrapMode: Label.WordWrap
                Layout.fillWidth: true

                text: qsTr("Sorry but your application version no longer support. Please upgrade your application!");
            }

            Button {
                text: qsTr("Upgrade");
                Layout.alignment: Qt.AlignCenter

                onClicked: {
                    const link = mainModel.storeLink();

                    if (link.length) {
                        Qt.openUrlExternally(link);
                    }
                }
            }
        }
    }

    // temp solution
    Component.onCompleted: {
        toolBar.enabled = false
    }
}
