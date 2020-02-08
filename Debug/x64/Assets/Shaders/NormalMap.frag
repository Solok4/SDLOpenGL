#version 330 core

in vec2 UV;
in vec3 NormalVec;
out vec4 FragColor;

uniform sampler2D Normal;

void main()
{
	vec4 NormalEl = texture(Normal,UV);
	NormalEl.z = NormalEl.z*2.0-1.0f;
	NormalEl.xyz*=NormalVec;

	FragColor = vec4(NormalEl.xyz,1.0f);
}