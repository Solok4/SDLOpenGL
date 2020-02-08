attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;
varying vec2 UV;
varying vec3 FragPos;

uniform mat4 depthMVP;
uniform mat4 Model;

void main(){
	gl_Position = depthMVP* Model* vec4(vertexPosition_modelspace,1);
	UV = vertexUV;
	FragPos = vertexNormal;
}