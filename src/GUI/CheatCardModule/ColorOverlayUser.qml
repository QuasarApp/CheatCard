import QtQuick 2.15

ShaderEffect {
    property variant src: Image

    property real r: 0.344
    property real g: 0.5
    property real b: 0.156

    vertexShader: "
        uniform highp mat4 qt_Matrix;
        attribute highp vec4 qt_Vertex;
        attribute highp vec2 qt_MultiTexCoord0;
        varying highp vec2 coord;

        void main() {
            coord = qt_MultiTexCoord0;
            gl_Position = qt_Matrix * qt_Vertex;
        }
    "

    fragmentShader: "
        varying highp vec2 coord;
        uniform sampler2D src;

        uniform lowp float r;
        uniform lowp float g;
        uniform lowp float b;

        void main() {
            lowp vec4 clr = texture2D(src, coord);
            lowp float avg = (clr.r + clr.g + clr.b) / 3.;
            gl_FragColor = vec4(r * avg, g * avg, b * avg, clr.a);
        }
    "
}
