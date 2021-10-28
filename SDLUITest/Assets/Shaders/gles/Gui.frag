#version 100
precision mediump float;

varying vec2 UV;
	
uniform sampler2D Tex;
uniform vec3 ColorMask;

void main()
{
    vec4 tex = texture2D(Tex,UV);
	gl_FragColor = tex * vec4(ColorMask,1);
} 