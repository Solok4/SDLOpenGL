
// Values that stay constant for the whole mesh.
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 CameraPos;
uniform mat4 NormalMatrix;

attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;
varying vec2 UV;
varying vec3 FragPos;
varying vec3 CameraP;
varying vec3 NormalVec;



  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  
  UV = vertexUV;
  NormalVec = mat3(NormalMatrix)*vertexNormal;
  FragPos = vec3(Model * vec4(vertexPosition_modelspace,1.0));
  CameraP = CameraPos;
}