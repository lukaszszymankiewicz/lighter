#version 330 core

uniform vec4 inColor;
out vec4 outColor;

void main()
{
    outColor = vec4(lightPolygonColor);
}
