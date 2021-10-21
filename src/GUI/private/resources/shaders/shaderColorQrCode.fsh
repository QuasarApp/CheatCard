varying highp vec2 coord;
uniform sampler2D src;

uniform lowp float r;
uniform lowp float g;
uniform lowp float b;

void main() {
    lowp vec4 clr = texture2D(src, coord);
    int fBlack = int(clr.r + clr.g + clr.b);
    if ( fBlack != 0) {
        gl_FragColor = vec4(0, 0, 0 , 0);
    } else {
        gl_FragColor = vec4(r, g , b , 1);
    }
}
