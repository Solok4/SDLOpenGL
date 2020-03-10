precision mediump float;

varying vec4 FragPos;

uniform vec3 LightPos;
uniform float FarPlane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - LightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / FarPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  