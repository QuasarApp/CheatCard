import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import com.scythestudio.scodes 1.0

Item {
    id: root

    property string inputText: ""
    property string fileName: "";

    property alias qrCodeFilePath: generator.filePath
    property alias customSource: imgQr.source

    Image {
        id: imgQr
        property int qrIndex: 0

        anchors.centerIn: parent
        height: Math.min(parent.height, parent.width)
        width: Math.min(parent.height, parent.width)
        fillMode: Image.PreserveAspectFit
        SBarcodeGenerator {
            id: generator
            fileName: root.fileName;
            inputText: root.inputText
            height: Math.max(parent.height, 200)
            width: Math.max(parent.width, 200)
            margin: 0
            onInputTextChanged: {
                if (inputText.length)
                    process(inputText);

                imgQr.qrIndex++
            }
        }
        source: "image://cards/file:" + generator.filePath + ":" + imgQr.qrIndex
        layer.enabled: true
        layer.effect: ShaderColorOverlay {
            color: Material.primary
            fragSh: "qrc:/private/resources/shaders/shaderColorQrCode.fsh"
        }

    }
}
