attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
  
// Values that stay constant for the whole mesh.
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

varying vec2 UV;
  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  mat4 MVP = Projection * View * Model;
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  
  UV = vertexUV;
}