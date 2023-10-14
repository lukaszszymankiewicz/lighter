#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

vec2 aPosNDC;

uniform vec2 aScale;
uniform vec2 aCamera;

out vec2 TexCoord;

void main()
{
    aPosNDC = vec2(
        ((2.0 * (aPos.x-aCamera.x) / aScale.x) - 1.0),
        (((2.0 * (aPos.y-aCamera.y) / aScale.y) - 1.0) * -1.0)
    );

    gl_Position = vec4(aPosNDC, 0.0, 1.0);

    TexCoord = aTexCoord;
}
