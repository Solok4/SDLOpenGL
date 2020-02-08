#version 330 core

in vec3 FragPos;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D Base;

void main()
{
	vec4 BaseEl = texture(Base,UV);

	FragColor = vec4(BaseEl.xyz,1.0f);
}