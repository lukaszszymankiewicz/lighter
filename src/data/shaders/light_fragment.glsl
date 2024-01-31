#version 330 core

uniform vec4  aColor;
uniform vec2  emit;
uniform float diffuse;

// this can make light effect pretty - play with it
float lenn (vec2 v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

void main() {
    float dist   = lenn(gl_FragCoord.xy - emit.xy);
    gl_FragColor = mix(aColor, vec4(0.0, 0.0, 0.0, 0.5), dist/diffuse);
}
