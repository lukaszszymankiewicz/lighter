#version 330 core

layout (location = 0) in vec2 aPos;

vec2 aPosNDC;

uniform vec2 aScale;
uniform vec2 aCamera;

void main()
{
    // aPosNDC = (2.0 * aPos / aScale) - 1.0;

    aPosNDC = vec2(
        ((2.0 * (aPos.x-aCamera.x) / aScale.x) - 1.0),
        (((2.0 * (aPos.y-aCamera.y) / aScale.y) - 1.0) * -1.0)
    );

    gl_Position = vec4(aPosNDC, 0.0, 1.0);
}
