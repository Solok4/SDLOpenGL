#version 330 core

#ifdef GL_ES

	varying vec2 UV;
	varying vec4 FragColor;
#else
	in vec2 UV;
	out vec4 FragColor;
#endif

uniform sampler2D Tex;

void main()
{
    //FragColor = vec4(UV, 0.2f, 1.0f);
	FragColor = texture(Tex,UV);
} 