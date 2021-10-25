varying highp vec2 coord;
uniform lowp sampler2D source;

uniform lowp float r;
uniform lowp float g;
uniform lowp float b;

void main() {
    lowp vec4 clr = texture2D(source, coord);
    lowp float avg = (clr.r + clr.g + clr.b) / 3.;
    gl_FragColor = vec4(r * avg, g * avg, b * avg, clr.a);
}
