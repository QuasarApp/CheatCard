//#
//# Copyright (C) 2018 - 2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QuasarAppCreditsModule 1.0


Credits {
    property var model: null
    state: "about"
    iconWidth: 150
    iconLogo: "qrc:/images/private/resources/Logo/CheatCard_Logo1.png"
    listCustomInfo: [
        {
            text: "**CheatCard**: " + ((model)? model.getCoreLibVr() : ""),
            align: Label.AlignHCenter,
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("**Third party libraries**"),
            align: Label.AlignHCenter,
        },
        {
            text: "Qt: " + ((model)? model.getQtLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: "Heart: " + ((model)? model.getHeartLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: "SimpleQmlNotify " + ((model)? model.getSimpleQmlNotifyLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: "DoctorPill: " + ((model)? model.getDoctorPillLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: "ZXing-C++ " + ((model)? model.getZxingCppLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: "SCodes " + ((model)? model.getSCodesLibVr() : ""),
            align: Label.AlignHCenter,
            color: "#777777"
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        },
        {
            text: qsTr("Developed by Quasar App Core team"),
            align: Label.AlignHCenter,
            color: "#777777"
        }
    ]

}
