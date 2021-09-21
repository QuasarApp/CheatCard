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

    ColumnLayout {

        Item {
            Layout.fillHeight: true
        }

        Label {
            text: qsTr("please wait until the exchange is successful  ")
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

        }

        BusyIndicator {
            running: true
            Layout.alignment: Qt.AlignHCenter

        }

        Item {
            Layout.fillHeight: true
        }

    }


}
