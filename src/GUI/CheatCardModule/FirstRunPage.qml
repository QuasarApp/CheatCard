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
                    text: qsTr("Who are you?");
                    font.pointSize: 30
                    color: "#424242"
                    wrapMode: Label.WordWrap
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

                        ColumnLayout {
                            Layout.alignment: Qt.AlignCenter

                            RadioButton {
                                checked: true
                                text: qsTr("I am client")
                            }

                            RadioButton {
                                id: rSaller
                                text: qsTr("I am seller")
                            }
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
                    text: (!rSaller.checked)? qsTr("What is your name?") :
                                             qsTr("What is the name of your company?");
                    font.pointSize: 30
                    color: "#424242"
                    wrapMode: Label.WordWrap
                }


                Keys.onReleased: {
                    if ((event.key === Qt.Key_Back ||
                        event.key === Qt.Key_Escape)) {
                        event.accepted = true
                        view.currentIndex--;
                    }
                }

                focus: true

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
                            placeholderText: qsTr("Please enter your name here")
                            horizontalAlignment: TextInput.AlignHCenter

                        }

                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            Button {
                                text: qsTr("Back")
                                Layout.alignment: Qt.AlignHCenter

                                onClicked: () => {
                                               view.currentIndex--;
                                           }

                            }


                            Button {
                                text: qsTr("Go!")

                                onClicked: () => {
                                               view.currentIndex = 2;
                                           }
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

                    if (rSaller.checked) {
                        object.becomeSellerRequest();
                    }

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
