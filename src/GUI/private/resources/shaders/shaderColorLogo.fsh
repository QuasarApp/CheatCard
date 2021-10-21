varying highp vec2 coord;
uniform sampler2D src;

uniform lowp float r;
uniform lowp float g;
uniform lowp float b;

void main() {
    lowp vec4 clr = texture2D(src, coord);
    lowp float avg = 1. - ((clr.r + clr.g + clr.b) / 3.);
    gl_FragColor = vec4(r * avg, g * avg, b * avg, clr.a);
}
