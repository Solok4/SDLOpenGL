#version 330 core

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D Base;

void main()
{
    outColor = texture(Base, Texcoord);
}