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


Credits {
    property var model: null
    state: "about"
    listCustomInfo: [
        {
            text: "### Как использовать?",
            align: Label.AlignHCenter,
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;Алгоритм действий для покупателя работает по системе:"),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Пришёл в заведение."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Сделал заказ."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Показал qr код в приложении на кассе."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Получил печать в карточку."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("* Повторил эту процедуру несколько раз и получил бонус установленный продавцом."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;Для того что бы открыть QR код нажмите на элемент в верхнем левом углу экрана. "),
            align: Label.AlignLeft
        },
        {
            text: qsTr("Help1.jpg"),
            align: Label.AlignHCenter,
//            sourceImg: "qrc:/private/resources/help/Help1.jpg"
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;Откроется сайдбар внизу которого будет QR код, вы можете нажать на него что бы увеличить."),
            align: Label.AlignLeft
        },
        {
            text: qsTr("&nbsp;&nbsp;&nbsp;&nbsp;После получения карты вы можете посмотреть дополнительную информацию о карте нажав на неё."),
            align: Label.AlignLeft
        },
        {
            text: qsTr(" "),
            align: Label.AlignHCenter
        }
    ]

}
