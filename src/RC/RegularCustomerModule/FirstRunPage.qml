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

    property var model: null

    signal finished();

    ColumnLayout {
        anchors.fill: parent


        SwipeView {
            id: view
            currentIndex: 0
            clip: true
            interactive: false

            Layout.fillWidth: true
            Layout.fillHeight: true

            Page {
                id: selectTypePage
                header: Label {
                    horizontalAlignment: Label.AlignHCenter
                    text: qsTr("Why you are?");
                    font.bold: true
                }

                contentItem: Item {
                    ButtonGroup {
                        buttons: column.children
                    }

                    ColumnLayout {
                        id: column
                        anchors.fill: parent

                        Item {
                            Layout.fillHeight: true
                        }

                        RadioButton {
                            checked: true
                            Layout.alignment: Qt.AlignHCenter
                            text: qsTr("I am client")
                        }

                        RadioButton {
                            id: rSaller

                            Layout.alignment: Qt.AlignHCenter
                            text: qsTr("I am saller")
                        }

                        Button {
                            text: qsTr("Next")
                            Layout.alignment: Qt.AlignHCenter

                            onClicked: () => {
                                           view.currentIndex++;
                                       }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            Page {
                id: selectName

                header: Label {
                    horizontalAlignment: Label.AlignHCenter
                    text: (rSaller.checked)? qsTr("What your name?") :
                                             qsTr("What name of your comapny?");
                    font.bold: true
                }


                contentItem: Item {

                    ColumnLayout {
                        anchors.fill: parent

                        Item {
                            Layout.fillHeight: true
                        }

                        TextField {
                            id: name
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            placeholderText: qsTr("Please Enter your name or name your company here")
                        }

                        Button {
                            text: qsTr("Next")
                            Layout.alignment: Qt.AlignHCenter

                            onClicked: () => {
                                           view.currentIndex = 2;
                                       }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            RegistrationFinishedPage {
                onFinished: {
                    if (!root.model)
                        return;


                    const object = root.model.currentUser;

                    object.name = name.text;
                    object.fSaller = rSaller.checked;

                    root.model.configureFinished();

                    root.finished();
                }
            }
        }

        PageIndicator {
            id: indicator
            Layout.alignment: Qt.AlignHCenter

            count: view.count
            currentIndex: view.currentIndex
            interactive: false
        }
    }
}
