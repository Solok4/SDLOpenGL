#version 330 core

// Values that stay constant for the whole mesh.
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 CameraPos;
uniform mat4 NormalMatrix;
uniform mat4 depthMVP[8];
uniform int LightCount;


in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;
out vec2 UV;
out vec3 FragPos;
out vec3 CameraP;
out vec3 NormalVec;
out vec4 LightMVP[8];
flat out int LCount;


  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  LCount = LightCount;
  UV = vertexUV;
  NormalVec = mat3(NormalMatrix)*vertexNormal;
  FragPos = vec3(Model * vec4(vertexPosition_modelspace,1.0));
  CameraP = CameraPos;
    for(int i=0;i<LightCount;i++)
  {
	LightMVP[i] = depthMVP[i] * vec4(FragPos,1);
  }
}