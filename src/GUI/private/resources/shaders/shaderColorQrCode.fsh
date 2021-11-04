varying highp vec2 coord;
uniform lowp sampler2D source;

uniform lowp float r;
uniform lowp float g;
uniform lowp float b;

void main() {
    lowp vec4 clr = texture2D(source, coord);
    int fBlack = int(clr.r + clr.g + clr.b);
    if ( fBlack != 0) {
        gl_FragColor = vec4(0, 0, 0 , 0);
    } else {
        gl_FragColor = vec4(r * 0.5, g * 0.5, b * 0.5, 1);
    }
}
