import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Item {
    id: root

    property rect captureRect
    property color overlayColor: Material.accent
    Item {
        id: captureZoneCorners
        x: root.captureRect.x
        y: root.captureRect.y
        width: root.captureRect.width
        height: root.captureRect.height

        Rectangle {
            anchors {
                top: parent.top
                left: parent.left
            }

            width: 20
            height: 5
            color: overlayColor
            radius: height / 2
        }

        Rectangle {
            anchors {
                top: parent.top
                left: parent.left
            }

            width: 5
            height: 20
            color: overlayColor
            radius: width / 2
        }

        // ----------------------
        Rectangle {
            anchors {
                bottom: parent.bottom
                left: parent.left
            }

            width: 20
            height: 5
            color: overlayColor
            radius: height / 2
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                left: parent.left
            }

            width: 5
            height: 20
            color: overlayColor
            radius: width / 2
        }

        // ----------------------
        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
            }

            width: 20
            height: 5
            color: overlayColor
            radius: height / 2
        }

        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
            }

            width: 5
            height: 20
            color: overlayColor
            radius: width / 2
        }

        // ----------------------
        Rectangle {
            anchors {
                bottom: parent.bottom
                right: parent.right
            }

            width: 20
            height: 5
            color: overlayColor
            radius: height / 2
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                right: parent.right
            }

            width: 5
            height: 20
            color: overlayColor
            radius: width / 2
        }

        Rectangle {
            id: scanIndicator

            y: captureZoneCorners.height / 2

            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width
            height: 1

            color: overlayColor

            SequentialAnimation {
                id: scanIndicatorAnimation
                loops: Animation.Infinite

                PropertyAnimation {

                    id: toTopAnimation
                    target: scanIndicator
                    property: "y"
                    easing.type: Easing.OutExpo
                    duration: 2000
                }

                PropertyAnimation {
                    id: toBottomAnimation
                    target: scanIndicator
                    property: "y"
                    easing.type: Easing.OutExpo

                    duration: 2000
                }
            }
        }

        RectangularGlow {
            id: effect

            anchors.centerIn: scanIndicator

            width: scanIndicator.width / 2
            height: scanIndicator.height

            glowRadius: 50
            spread: 0.2
            color: overlayColor
            cornerRadius: glowRadius
        }
    }

    Label {
        id: scanCapsuleText

        anchors {
            verticalCenter: captureZoneCorners.bottom
            horizontalCenter: captureZoneCorners.horizontalCenter
        }

        text: qsTr("Scan QR code")
        color: overlayColor
    }

    onCaptureRectChanged: {
        scanIndicatorAnimation.stop()

        toTopAnimation.to = 5
        toBottomAnimation.to = captureRect.height - 5

        scanIndicatorAnimation.start()
    }
}
