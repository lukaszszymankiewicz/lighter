#version 330 core

uniform vec4 aColor;
uniform vec2 emit;
uniform vec2 aScale;
uniform vec2 aCamera;
uniform float power;

out vec4 outColor;

// this can make light effect pretty - play with it
float lenn (vec2 v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

void main() {
    // third version

    // x0 + scale_arr.values[0] /2.0 - (float)camera_arr.values[0],

    vec2 emit_ndc = emit + aScale / 2.0 - aCamera;
    vec2 pos_ndc = 2.0 * gl_FragCoord.xy / emit_ndc.xy - 1.0;

    float dist = lenn(pos_ndc);
    outColor = mix(aColor, vec4(0.0, 0.0, 0.0, 1.0), dist*power);
    
    // fourth version
    // vec2 pos_ndc = 2.0 * gl_FragCoord.xy / vec2(320.0, 240.0) - 1.0;
    // float dist = distance(pos_ndc, emit.xy);
    // outColor = mix(aColor, vec4(0.0, 0.0, 0.0, 1.0), dist*2.5);
}
