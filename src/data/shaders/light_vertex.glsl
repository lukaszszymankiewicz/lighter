#version 330 core

in vec2 aPos;

uniform vec2 aCamera;
uniform mat2 aScale;

void main()
{
    gl_Position = vec4((aPos - aCamera) * aScale, 0.0, 1.0);
}
