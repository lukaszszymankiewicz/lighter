#version 330 core

uniform vec4 lightColor;
out vec4 outColor;

void main()
{
    outColor = vec4(lightColor);
}
