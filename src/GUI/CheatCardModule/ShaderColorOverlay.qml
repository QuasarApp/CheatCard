import QtQuick 2.15

ShaderEffect {
    property string vrt: "qrc:/private/resources/shaders/baseVertxShader.vsh"
    property string fragSh: ""
    property variant src: Image
    property color colorQr: color

    property real r: colorQr.r
    property real g: colorQr.g
    property real b: colorQr.b

    vertexShader: vrt
    fragmentShader: fragSh
}
