#version 330 core

uniform vec4 aColor;
uniform vec2 emit;
uniform float diffuse;

out vec4 outColor;

// this can make light effect pretty - play with it
float lenn (vec2 v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

void main() {
    vec2 pos_ndc = 2.0 * gl_FragCoord.xy / emit.xy - 1.0;
    float dist = lenn(pos_ndc);
    outColor = mix(aColor, vec4(0.0, 0.0, 0.0, 1.0), dist*diffuse);
}
