#version 330 core

in vec2 aPos;

uniform vec2 aCamera;

void main()
{
    gl_Position = vec4((aPos - aCamera), 0.0, 1.0);
}
