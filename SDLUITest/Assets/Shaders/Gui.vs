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

#ifdef GL_ES
	varying vec2 UV;
#else
	out vec2 UV;
#endif
  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  
  UV = vertexUV;
}