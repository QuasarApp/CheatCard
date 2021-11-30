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

ColumnLayout {
    id: root

    property var model: null

    CardsListView {

        Layout.fillHeight: true
        Layout.fillWidth: true
        model: (root.model) ? root.model.cardsList: null


        editable: Boolean(root.model && root.model.mode)
        visible: !firstRun.visible
    }

    FirstRunPage {
        id: firstRun
        visible: (model)? model.fFirst : false

        Layout.fillHeight: true
        Layout.fillWidth: true
        model: mainModel

    }
}

