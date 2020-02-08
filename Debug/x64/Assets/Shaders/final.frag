#version 330 core

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D BaseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D LightMap;
uniform sampler2D DepthMap;


void main()
{
	vec4 BaseEl = texture(BaseMap,Texcoord);
	vec4 NormalEl = texture(NormalMap,Texcoord);
	vec4 SpecularEl = texture(SpecularMap,Texcoord);
	vec4 LightEl = texture(LightMap,Texcoord);
	vec4 DepthEl = texture(DepthMap,Texcoord);

	vec4 BaseElxNormalEl = mix(BaseEl,NormalEl,1.0);
	vec4 BaseElxSpecularEl = mix(BaseEl,SpecularEl,1.0);
	vec4 BaseElxLightEl = mix(BaseEl,LightEl,1.0);
	vec4 BaseElxDepthEl = mix(BaseEl,DepthEl,1.0);


    outColor = vec4(BaseElxDepthEl.xyz,1.0);
}