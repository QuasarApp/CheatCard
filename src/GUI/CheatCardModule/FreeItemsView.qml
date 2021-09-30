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

    title: qsTr("It seems you deserve a bonus.");

    header: Label {
        horizontalAlignment: Label.AlignHCenter
        text: title
        font.bold: true
        visible: text.length
    }

    Label {
        text: qsTr("You have ") + freeItems + " free's " + ((root.model)? root.model.freeItem : "");
    }

    footer: DialogButtonBox {
        onAccepted: () => {
                        activityProcessor.popItem();
                    }

        standardButtons: Dialog.Ok
    }
}
