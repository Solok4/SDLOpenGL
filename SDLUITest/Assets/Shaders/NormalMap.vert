#version 330 core

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec2 UV;
out vec3 NormalVec;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 NormalMatrix;

void main()
{
	mat4 MVP = Projection*View*Model;
	gl_Position = MVP*vec4(vertexPosition_modelspace,1.0f);
	 NormalVec = vertexNormal;
	 UV = vertexUV;
}