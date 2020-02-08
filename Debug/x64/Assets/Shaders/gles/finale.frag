precision mediump float;

varying vec2 Texcoord;
uniform sampler2D Base;

void main()
{
    gl_FragColor = texture2D(Base, Texcoord);
}