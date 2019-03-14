#version 330 core
#ifdef GL_ES
	varying vec2 position;
	varying vec2 texcoord;
	varying vec2 Texcoord;
#else
	in vec2 position;
	in vec2 texcoord;
	out vec2 Texcoord;
#endif
void main()
{
    Texcoord = texcoord;
    gl_Position = vec4(position, 0.0, 1.0);
}