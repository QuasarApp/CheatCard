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
import QtGraphicalEffects 1.15

import "Style"

RowLayout {
    id: root
    property alias textField: source
    property string lineColor: "black"
    property alias image: img.source

    signal clicked(var mouse)

    Image {
        Layout.preferredHeight: source.height * 0.9
        Layout.preferredWidth: height
        mipmap: true;

        MouseArea {

            onClicked: (mouse) => {
                            root.clicked(mouse)
                        }

            anchors.fill: parent
        }

        id: img
        fillMode: Image.PreserveAspectFit

        ColorOverlay {
            anchors.fill: img
            source: img
            color: lineColor
        }
    }

    CTextField {
        id: source

        fontColor: lineColor
    }
}
