#version 330 core

layout (location = 0) in vec2 aPos;

vec2 aPosNDC;

uniform vec2 aCamera;
uniform vec2 aScale;

void main()
{
    aPosNDC = (2.0 * (aPos - aCamera) / aScale) - 1.0;
    gl_Position = vec4(aPosNDC, 0.0, 1.0);
}
