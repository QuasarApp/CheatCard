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
import QtGraphicalEffects 1.0

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

    TextField {
        id: source

        background: Rectangle {
            y: source.height - height - source.bottomPadding + 8
            implicitWidth: 120
            height: source.activeFocus || source.hovered ? 2 : 1
            color: source.activeFocus ? source.Material.accentColor
                                       : (source.hovered ? source.Material.primaryTextColor : lineColor)
        }
    }
}
