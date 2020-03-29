#version 330 core

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec2 UV;

void main(){
	 mat4 MVP = Projection * View * Model;
	 gl_Position = MVP* vec4(vertexPosition_modelspace,1);
	 UV = vertexUV;
}