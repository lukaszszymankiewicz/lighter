#version 330 core

uniform vec2      aScale;
uniform vec4      aColor;
uniform vec2      emit;
uniform float     diffuse;
uniform sampler2D ourTexture;


// this can make light effect pretty - play with it
float lenn (vec2 v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

void main() {
    float dist       = lenn(gl_FragCoord.xy - emit.xy);

    vec2 TexCoord = gl_FragCoord.xy / aScale - 1;
    vec4 TexColor = texture2D(ourTexture, TexCoord);

    gl_FragColor        = mix(vec4(TexColor.rgb, 0.5), aColor, dist/diffuse);
}
