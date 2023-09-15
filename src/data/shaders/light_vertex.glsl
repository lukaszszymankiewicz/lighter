#version 330 core

in vec2 position;

uniform vec2 aCamera;

void main()
{
    gl_Position = vec4(position.x-aCamera.x, position.y-aCamera.y, 0.0, 1.0);
}
