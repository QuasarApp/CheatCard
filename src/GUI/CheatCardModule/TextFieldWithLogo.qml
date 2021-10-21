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

RowLayout {
    id: root
    property alias textField: source
    property string lineColor: "#777777"
    property alias image: img.source

    signal clicked(var mouse)

    Image {
        id: img
        Layout.preferredHeight: source.height * 0.9
        Layout.preferredWidth: height
        fillMode: Image.PreserveAspectFit
        mipmap: true;

        layer.enabled: true
        layer.effect: ShaderColorOverlay {
            src: img
            color: lineColor
            fragSh: "qrc:/private/resources/shaders/shaderColorLogo.fsh"
        }

        MouseArea {

            onClicked: (mouse) => {
                            root.clicked(mouse)
                        }

            anchors.fill: parent
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
