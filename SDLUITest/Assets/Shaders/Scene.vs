#version 330 core
#ifdef GL_ES
	attribute vec3 vertexPosition_modelspace;
	attribute vec2 vertexUV;
#else
	layout(location = 0) in vec3 vertexPosition_modelspace;
	layout(location = 1) in vec2 vertexUV;
#endif

// Values that stay constant for the whole mesh.
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 CameraPos;

#ifdef GL_ES
	varying vec2 UV;
	varying vec3 FragPos;
	varying vec3 CameraP;
#else
	out vec2 UV;
	out vec3 FragPos;
	out vec3 CameraP;
#endif
  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  
  UV = vertexUV;
  FragPos = vec3(Model * vec4(vertexPosition_modelspace,1.0));
  CameraP = CameraPos;
}