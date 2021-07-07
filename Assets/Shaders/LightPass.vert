#version 330 core

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec2 UV;
out vec3 OutNormal;
out vec3 FragPos;
out vec4 LightMVP[8];
out vec3 CameraP;
flat out int LCount;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform vec3 Camera;
uniform mat4 depthMVP[8];
uniform int LightCount;


void main()
{
	mat4 MVP = Projection*View*Model;
	gl_Position = MVP*vec4(vertexPosition_modelspace,1.0f);
	 OutNormal = mat3(NormalMatrix)*vertexNormal;
	 FragPos = vec3(Model*vec4(vertexPosition_modelspace,1.f));
	 UV = vertexUV;
	 LCount = LightCount;
	for(int i=0;i<LightCount;i++)
	{
		LightMVP[i] = depthMVP[i] * vec4(FragPos,1.);
	}
	CameraP = Camera;
}