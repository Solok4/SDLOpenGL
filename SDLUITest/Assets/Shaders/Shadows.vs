#version 330 core

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

uniform mat4 depthMVP;
uniform mat4 Model;

out vec2 UV;
out vec3 Normal;

void main(){
	gl_Position = depthMVP* Model* vec4(vertexPosition_modelspace,1);
	UV = vertexUV;
	Normal = vertexNormal;
}