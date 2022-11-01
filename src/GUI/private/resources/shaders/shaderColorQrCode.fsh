varying highp vec2 coord;
uniform lowp sampler2D source;

uniform lowp float r;
uniform lowp float g;
uniform lowp float b;

void main() {
    lowp vec4 clr = texture2D(source, coord);
    bool fBlack = (clr.r + clr.g + clr.b) / 3.0 < 0.2;
    bool fGray = (clr.r + clr.g + clr.b) / 3.0 < 0.80;

    if ( fBlack ) {
        gl_FragColor = vec4(r * 0.5, g * 0.5, b * 0.5, 1);
    } else if ( fGray ) {
        gl_FragColor =  vec4(clr.r * r, clr.g * g, clr.b * b, 1);
    } else {
        gl_FragColor = vec4(1, 1, 1 , 1);
    }
}
