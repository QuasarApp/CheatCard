import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Image {
    id: imgQr
    layer.enabled: true
    layer.effect: ShaderColorOverlay {
        color: Material.primary
        fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
    }
}

