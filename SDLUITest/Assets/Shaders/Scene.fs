#version 330 core

#ifdef GL_ES
	precision mediump float;
	
	attribute uniform sampler2D Base;
	attribute uniform sampler2D Normal;
	attribute uniform sampler2D Specular;
	
	varying vec2 UV;
	varying vec3 FragPos;
	varying vec3 CameraP;

#else

	layout(binding=0) uniform sampler2D Base;
	layout(binding=1) uniform sampler2D Normal;
	layout(binding=2) uniform sampler2D Specular;

	in vec2 UV;
	in vec3 FragPos;
	in vec3 CameraP;

	out vec4 FragColor;
#endif

void main()
{
	vec3 LightPosition = vec3(10.0,10.0,0.0);
	vec3 ColorLight = vec3(1.0,1.0,0.0);
	
	//ambient
	float AmbientStrength = 0.1;
	vec3 Ambient = AmbientStrength*ColorLight;
	
	
	vec4 BaseEl = texture(Base,UV);
	vec4 NormalEl = texture(Normal,UV);
	vec4 SpecularEl = texture(Specular,UV);
	
	//diffuse
	vec3 norm = normalize(NormalEl.xyz);
	vec3 LightDir = normalize(LightPosition-FragPos);
	float diff = max(dot(norm,LightDir),0.0);
	vec3 diffuse = diff*ColorLight;
	
	//specular
	float specularStrength = 0.5;
	
	vec3 viewDir = normalize(CameraP-FragPos);
	vec3 reflectDir = reflect(-LightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),SpecularEl.x);
	vec3 specular = specularStrength* spec * ColorLight;
	
	vec3 result = (Ambient+diffuse+specular) * BaseEl.xyz;

	FragColor = vec4(result,1.0);
} 