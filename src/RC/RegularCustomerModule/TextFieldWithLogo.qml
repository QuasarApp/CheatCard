//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts


RowLayout {
    property alias textField: source
    property alias image: img.source

    Image {
        Layout.preferredHeight: source.height * 0.9
        Layout.preferredWidth: height

        id: img
        fillMode: Image.PreserveAspectFit
    }

    TextField {
        id: source
    }
}
