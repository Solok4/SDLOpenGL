#version 150 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D defaultTex;
uniform sampler2D guiTex;
void main()
{
    outColor = texture(defaultTex, Texcoord);
}