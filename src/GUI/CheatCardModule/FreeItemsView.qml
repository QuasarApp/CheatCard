//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Page {
    id: root
    property int freeItems: 0
    property var model: null

    title: qsTr("Wooooh seems like someone has %0 free %1 !!!").
    arg(freeItems).
    arg(((root.model)? root.model.freeItem : ""))

    contentItem: ColumnLayout {

        Label {
            horizontalAlignment: Label.AlignHCenter
            text: title
            font.bold: true
            visible: text.length
            font.pointSize: 15
            wrapMode: Label.WordWrap
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
        }

        Image {
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: h
            Layout.preferredWidth: w

            property int w:  Math.min(root.width, root.height) * 0.5
            property int h: Math.min(root.width, root.height) * 0.5

            source: "image://cards/seal/" +
                    ((root.model)? root.model.id : "0")

            opacity: 0

            Component.onCompleted:  {
                opacity = 1;
            }

            Behavior on w {
                NumberAnimation {
                    duration:  1000
                    easing.type: Easing.OutBounce
                }
            }

            Behavior on h {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        }

    }

    footer: DialogButtonBox {
        onAccepted: () => {
                        activityProcessor.popItem();
                    }

        standardButtons: Dialog.Ok
    }
}
