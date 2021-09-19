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

ColumnLayout {
    id: root

    property var model: null


    SwipeView {
        id: view

        clip: true
        interactive: user && user.fSaller
        currentIndex: 0

        onCurrentIndexChanged: () => {
                                   if (root.model) {
                                       root.model.mode = user && user.fSaller && currentIndex === 0;
                                   }
                               }

        Layout.fillWidth: true
        Layout.fillHeight: true

        CardsListView {
            model: (root.model) ? root.model.ownCardsList: null
            visible: user && user.fSaller

            editable: user && user.fSaller
        }

        CardsListView {
            model: (root.model) ? root.model.cardsList: null
            editable: false
        }

    }

    PageIndicator {
        id: indicator
        Layout.alignment: Qt.AlignHCenter
        visible: user && user.fSaller
        count: view.count
        currentIndex: view.currentIndex
        interactive: view.interactive

    }
}
