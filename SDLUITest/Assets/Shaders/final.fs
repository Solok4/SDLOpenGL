#version 330 core
#ifdef GL_ES
	attribute uniform sampler2D defaultTex;
	varying vec2 Texcoord;
	varying vec4 outColor;
#else
	layout(binding=0) uniform sampler2D defaultTex;
	in vec2 Texcoord;
	out vec4 outColor;
#endif

void main()
{
    outColor = texture(defaultTex, Texcoord);
}