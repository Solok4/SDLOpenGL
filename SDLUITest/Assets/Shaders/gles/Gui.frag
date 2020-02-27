precision mediump float;

varying vec2 UV;
	
uniform sampler2D Tex;

void main()
{
    //FragColor = vec4(UV, 0.2f, 1.0f);
	gl_FragColor = texture2D(Tex,UV);
} 