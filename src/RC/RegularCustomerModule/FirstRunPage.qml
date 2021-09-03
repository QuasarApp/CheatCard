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

Page {
    id: root
    title: qsTr("Registration");

    SwipeView {
        id: view

        currentIndex: 1
        anchors.fill: parent

        Page {
            id: selectTypePage
            title: qsTr("Why you are?");

            contentItem: Frame {
                ButtonGroup {
                    buttons: column.children
                }

                ColumnLayout {
                    id: column
                    anchors.fill: parent
                    RadioButton {
                        checked: true
                        text: qsTr("I am client")
                    }

                    RadioButton {
                        id: rSaller
                        text: qsTr("I am saller")
                    }
                }
            }
        }

        Page {
            id: selectName
            title: (rSaller.checked)? qsTr("What your name?") :
                                      qsTr("What name of your comapny?");

            contentItem: Frame {

                ColumnLayout {
                    anchors.fill: parent

                    TextField {
                        id: name
                    }
                }
            }
        }


        EditCardView {
            id: settingsSallerCard
        }
    }

    PageIndicator {
        id: indicator

        count: view.count
        currentIndex: view.currentIndex

        anchors.bottom: view.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Item {
        id: privateRoot

        property bool fSaller: false
    }

}
