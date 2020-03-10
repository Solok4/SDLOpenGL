precision mediump float;

varying vec2 Texcoord;
uniform vec3 Color;

void main()
{
   gl_FragColor = vec4(Color,1.0);
}