#version 330 core

uniform vec4 lightPolygonColor;
out vec4 outColor;

void main()
{
    outColor = vec4(lightPolygonColor);
}
