//#
//# Copyright (C) 2018 - 2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QuasarAppCreditsModule 1.0
import NotifyModule 1.0


Credits {
    property var model: null
    state: "about"
    iconWidth: 150
    iconLogo: "qrc:/images/private/resources/Logo/CheatCard_Logo1.png"
    developersList: [
        {
            section: qsTr("## QuasarApp Core team:"),
            sectionList: [
                "* Yankovich Andrei " + qsTr("(Programmer)"),
                "* Loschinin Igor " + qsTr("(Programmer)"),
                "* Chernenkov Oleg " + qsTr("(Designer)"),
            ]
        }
    ]

    versionList: [
        {
            section: qsTr("## Used Libraris 1"),
            sectionList: [
                "* CheatCard " + ((model)? model.getCoreLibVr() : ""),
            ]
        },
        {
            section: qsTr("## Used Libraris 2"),
            sectionList: [
                "* Heart " + ((model)? model.getCoreLibVr() : ""),
                "<a href='https://github.com/nu-book/zxing-cpp'>" + "* ZXing-C++ " + "</a> " + ((model)? model.getZxingCppLibVr() : ""),
                "<a href='https://github.com/QuasarApp/Heart'>" + "* SCodes " + "</a> " + ((model)? model.getSCodesLibVr() : ""),
            ]
        }
    ]
}
