#version 330 core

uniform vec4 aColor;

out vec4 outColor;

void main()
{
    outColor = vec4(aColor);
}
