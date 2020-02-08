attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normals;
varying vec2 Texcoord;
varying vec3 OutNormals;

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