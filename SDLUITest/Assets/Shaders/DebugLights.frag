#version 330 core

in vec2 Texcoord;
out vec4 outColor;
uniform vec3 Color;

void main()
{
   outColor = vec4(Color,1.f);
}