#version 330 core

in vec3 position;
in vec2 texcoord;
in vec3 normals;
out vec2 Texcoord;
out vec3 OutNormals;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	mat4 MVP = Projection * View * Model;
	gl_Position = MVP * vec4(position, 1.0);
    Texcoord = texcoord;
	OutNormals = normals;
}