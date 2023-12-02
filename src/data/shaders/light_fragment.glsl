#version 330 core

uniform vec4 aColor;
uniform vec2 emit;

out vec4 outColor;

void main()
{
    // third version
    // vec2 pos_ndc = 2.0 * gl_FragCoord.xy / emit.xy - 1.0;
    // float dist = length(pos_ndc);
    // outColor = mix(aColor, vec4(0.0, 0.0, 0.0, 1.0), dist*1.5);

    // aPosNDC = (2.0 * (aPos-aCamera) / aScale) - 1.0;
    // aPosNDC = (2.0 * (aPos-aCamera) / aScale) - 1.0;

    // second version
    // vec2 em_ndc = 2.0 * emit.xy / vec2(320.0, 240.0) - 1.0;
    // vec2 pos_ndc = 2.0 * (gl_FragCoord.xy - emit) / vec2(320.0, 240.0) - 1.0;
    // vec2 pos_ndc = vec4(1.0 - length(gl_FragCoord.xy - emit) / 800.0);
    // vec4 lg = vec4(1.0 - length(gl_FragCoord.xy - emit) / 800.0);
    // outColor = mix(aColor, lg, 0.25);
    // float dist = length(pos_ndc);

    // somewhat woking wersion
    // vec4 lg = vec4(1.0 - length(gl_FragCoord.xy - emit) / 800.0);
    // outColor = mix(aColor, lg, 0.25);

    outColor = aColor;
}
