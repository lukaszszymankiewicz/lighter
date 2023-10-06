#version 330 core

layout (location = 0) in vec2 aPos;

vec2 aPosNDC;
vec2 aCameraNDC;

uniform vec2 aCamera;
uniform vec2 aScale;

void main()
{
    aPosNDC = (2.0 * aPos / aScale) - 1.0;
    aCameraNDC = (2.0 * aCamera / aScale) - 1.0;
    gl_Position = vec4((aPosNDC  - aCameraNDC), 0.0, 1.0);
}
