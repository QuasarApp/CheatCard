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

import "Style"

CPage {
    id: root

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

            CPage {
                implicitWidth: 0x0

                id: selectTypePage
                title: qsTr("Who are you?");

                contentItem: Item {
                    ButtonGroup {
                        buttons: column.children
                    }

                    ColumnLayout {
                        id: column
                        anchors.fill: parent

                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                            RadioButton {
                                id: fClient
                                checked: true
                                text: qsTr("I am client")
                            }

                            RadioButton {
                                id: fSaller
                                text: qsTr("I am a business representative")
                                visible: model && model.fBillingAwailable()
                            }

                            RadioButton {
                                id: fRestore
                                text: qsTr("I just want to recover data")
                            }
                        }
                    }
                }
            }

            CPage {
                id: confugurePage
                implicitWidth: 0x0

                property bool done: name.text.length

                Keys.onReleased: {
                    if ((event.key === Qt.Key_Back ||
                         event.key === Qt.Key_Escape)) {
                        event.accepted = true
                        view.currentIndex--;
                    }
                }

                focus: true

                title: (!fSaller.checked)? qsTr("What is your name?") :
                                           qsTr("What is the name of your company?");

                contentItem: Item {

                    ColumnLayout {
                        anchors.fill: parent
                        visible: fClient.checked || fSaller.checked


                        TextField {
                            id: name
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.fillWidth: true
                            placeholderText: qsTr("Please enter your name here")
                            horizontalAlignment: TextInput.AlignHCenter

                        }
                    }

                    ImportUserKeyPage {
                        implicitWidth: 0x0
                        model: (root.model)? root.model.exportImportModel : null

                        anchors.fill: parent
                        visible: fRestore.checked

                        onImportFinished: {
                            root.finished();
                        }

                    }
                }
            }

            AppearanceCustomPage {
                title: qsTr("Customize the appearance of your application.")
            }

            RegistrationFinishedPage {
                implicitWidth: 0x0
                onFinished: (backUp) => {
                                if (!root.model)
                                return;

                                const object = root.model.currentUser;

                                object.name = name.text;

                                if (fSaller.checked) {
                                    object.becomeSellerRequest();
                                }

                                root.model.configureFinished();

                                if (backUp) {
                                    activityProcessor.newActivity("qrc:/CheatCardModule/ExportUserKeyPage.qml",
                                                                  mainModel.currentUser);

                                }

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

    footer: RowLayout {
        Layout.alignment: Qt.AlignHCenter
        visible: view.currentIndex < 3
        Button {
            text: qsTr("Back")
            Layout.alignment: Qt.AlignHCenter
            enabled: view.currentIndex
            onClicked: () => {
                           view.currentIndex--;
                           if (view.currentIndex === 0) {
                               name.text = ""
                           }
                       }
        }

        Button {
            id: nextButton
            Layout.alignment: Qt.AlignHCenter
            enabled: confugurePage.done || view.currentIndex != 1
            text: (view.currentIndex < 3)? qsTr("Next"): qsTr("Done!")
            onClicked: () => {
                           view.currentIndex++;
                       }
        }
    }
}
