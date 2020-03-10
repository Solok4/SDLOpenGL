#version 330 core
attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;

varying vec2 UV;
varying vec3 Normal;

uniform mat4 Model;

void main()
{
    gl_Position = Model * vec4(vertexPosition_modelspace, 1.0);
	UV = vertexUV;
	Normal = vertexNormal;
}  