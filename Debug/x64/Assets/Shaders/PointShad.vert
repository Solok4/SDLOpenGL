#version 330 core
in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec2 UV;
out vec3 Normal;

uniform mat4 Model;

void main()
{
    gl_Position = Model * vec4(vertexPosition_modelspace, 1.0);
	UV = vertexUV;
	Normal = vertexNormal;
}  