//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import Qt3D.Render 2.15
import QtMultimedia 5.15
import com.scythestudio.scodes 1.0

Page {
    id: root
    signal captured(var data)

    Camera {
        id: camera

        property int deviceIndex: 0
        property var devices: QtMultimedia.availableCameras
        property string defaultCamera: (devices.length)? devices[deviceIndex  % devices.length].deviceId : ""

        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }

        deviceId: (deviceIndex)? defaultCamera : config.getStrValue("cameraDevice", defaultCamera);

    }

    VideoOutput {
        id: videoOutput
        source: camera
        autoOrientation: true
        fillMode: VideoOutput.PreserveAspectCrop
        anchors.fill: parent
        // add barcodeFilter to videoOutput's filters to enable catching barcodes
        filters: [barcodeFilter]

        onSourceRectChanged: {
          barcodeFilter.captureRect = videoOutput.mapRectToSource(
                videoOutput.mapNormalizedRectToItem(Qt.rect(0.1, 0.1, 0.8, 0.8)))
        }

        ScannerOverlay {
            id: scannerOverlay
            anchors.fill: parent

            captureRect: videoOutput.mapRectToItem(barcodeFilter.captureRect)
        }

        // used to get camera focus on touched point
        MouseArea {
            anchors.fill: parent
            onClicked: {

                camera.focus.customFocusPoint = Qt.point(mouse.x / width,
                                                         mouse.y / height)
                camera.focus.focusMode = CameraFocus.FocusMacro
                camera.focus.focusPointMode = CameraFocus.FocusPointCustom
            }
        }
    }

    SBarcodeFilter {
        id: barcodeFilter

        // you can adjust capture rect (scan area) ne changing these Qt.rect() parameters
        captureRect: videoOutput.mapRectToSource(
                       videoOutput.mapNormalizedRectToItem(Qt.rect(0.1, 0.1,
                                                                   0.8, 0.8)))

        onCapturedChanged: (captured) => {
            root.captured(captured)
        }
    }

    ToolButton {
        text: qsTr("switch camera")

        anchors.horizontalCenter: parent.horizontalCenter

        anchors.bottom: parent.bottom
        icon.source: "qrc:/images/private/resources/Interface_icons/Camera_switch.svg"
        icon.color: Material.accent

        onClicked:  {
            camera.deviceIndex++
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
