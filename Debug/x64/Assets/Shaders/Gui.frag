#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D Tex;
uniform vec3 ColorMask;

void main()
{
	FragColor = texture(Tex,UV)*vec4(ColorMask,1.0f);
} 