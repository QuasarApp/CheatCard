//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "Style"

RowLayout {
    id: root
    property alias textField: source
    property string lineColor: "#777777"
    property alias image: img.source

    signal clicked(var mouse)

    Image {
        id: img
        asynchronous: true
        Layout.preferredHeight: source.height * 0.9
        Layout.preferredWidth: height
        fillMode: Image.PreserveAspectFit
        mipmap: true;
        layer.enabled: true
        layer.effect: ShaderColorOverlay {
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

    CTextField {
        id: source

        onPressed: {
            ToolTip.show(source.text)
        }

        onReleased: {
            ToolTip.hide()
        }

        fontColor: lineColor
    }
}
