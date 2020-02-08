#version 330 core

in vec3 FragPos;
in vec2 UV;
out vec4 FragColor;

uniform sampler2D Specular;

void main()
{
	vec4 SpecularEl = texture(Specular,UV);

	FragColor = vec4(SpecularEl.xyz,1.0f);
}