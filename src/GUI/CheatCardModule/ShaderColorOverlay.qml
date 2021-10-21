import QtQuick 2.15

ShaderEffect {
    property string vrt: "qrc:/private/resources/shaders/baseVertxShader.vsh"
    property string fragSh: ""
    property variant src: Image
    property color color: "#000000"

    property real r: color.r
    property real g: color.g
    property real b: color.b

    vertexShader: vrt
    fragmentShader: fragSh
}
