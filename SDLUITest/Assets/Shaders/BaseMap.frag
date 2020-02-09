#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuse;

in vec3 FragPos;
in vec2 UV;
in vec3 OutNormal;

uniform sampler2D Base;
uniform sampler2D Normal;
uniform sampler2D Specular;

void main()
{
	vec3 Normal = texture(Normal,UV).xyz;
	Normal.z= Normal.z*2.0f-1.0f;
	Normal *=OutNormal;

	gPosition = FragPos;
	gDiffuse.xyz = texture(Base,UV).xyz;
	gNormal = Normal;
	gDiffuse.z = texture(Specular,UV).x;
}