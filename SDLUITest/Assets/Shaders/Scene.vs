#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// Values that stay constant for the whole mesh.
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 CameraPos;
uniform mat4 NormalMatrix;

out vec2 UV;
out vec3 FragPos;
out vec3 CameraP;
out vec3 NormalVec;


  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  
  UV = vertexUV;
  NormalVec = mat3(NormalMatrix)*vertexNormal;
  FragPos = vec3(Model * vec4(vertexPosition_modelspace,1.0));
  CameraP = CameraPos;
}