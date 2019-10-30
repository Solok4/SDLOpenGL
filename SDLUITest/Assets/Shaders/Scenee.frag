precision mediump float;

struct Light
{
	vec3 Position;
	vec3 Rotation;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Color;

	int LightType;
	float Constant;
	float Linear;
	float Quadratic;

	float CutoutDist;
};

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

varying vec2 UV;
varying vec3 FragPos;
varying vec3 CameraP;
varying vec3 NormalVec;


uniform Material Mat;
uniform Light Lights[32];

uniform sampler2D Base;
uniform sampler2D Normal;
uniform sampler2D Specular;

vec3 ProccessDirectionalLight(Light l,vec4 BaseTex, vec4 NormalTex,vec4 SpecularTex)
{
	//ambient
	vec3 Ambient = vec3(0.1)*l.Color;
	
	//Diffuse
	//NormalTex.xyz = NormalTex.xyz*2.0-1.0;
	vec3 norm = normalize(NormalVec*NormalTex.xyz);
	vec3 LightDir = normalize(l.Rotation);
	float diff = max(dot(norm,LightDir),0.0);
	vec3 diffuse = (diff*Mat.Diffuse*l.Diffuse)*l.Color;
	
	//Specular
	vec3 viewDir = normalize(CameraP-FragPos);
	vec3 reflectDir = reflect(-LightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),Mat.Shininess);
	vec3 specular = (Mat.Specular*spec*l.Specular)*l.Color;
	
	//return ((Ambient+diffuse+specular)* BaseTex.xyz);
	return (Ambient)* BaseTex.xyz;
}

vec3 ProccessPointLight(Light l,vec4 BaseTex, vec4 NormalTex,vec4 SpecularTex)
{

	float distance = length(l.Position - FragPos);
	float attenuation = 1.0/(l.Constant+l.Linear*distance+l.Quadratic*(distance*distance));
	
	//ambient
	vec3 Ambient = vec3(0.1)*l.Color;
	Ambient *= attenuation;
	
	//Diffuse
	vec3 norm = normalize(NormalTex.xyz*NormalVec);
	vec3 LightDir = normalize(l.Position - FragPos);
	float diff = max(dot(norm,LightDir),0.0);
	vec3 diffuse = (diff*Mat.Diffuse)*l.Color;
	diffuse *= attenuation;
	
	//Specular
	vec3 viewDir = normalize(CameraP-FragPos);
	vec3 reflectDir = reflect(-LightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),Mat.Shininess);
	vec3 specular = (Mat.Specular*spec)*l.Color;
	specular *= attenuation;
	
	return (Ambient+diffuse+specular)* BaseTex.xyz;
}

vec3 ProccessSpotLight(Light l,vec4 BaseTex, vec4 NormalTex,vec4 SpecularTex)
{
	//ambient
	vec3 Ambient = vec3(0.1)*l.Color;
	
	//Diffuse
	vec3 norm = normalize(NormalTex.xyz*NormalVec);
	//vec3 LightDir = normalize(l.Position - FragPos);
	vec3 LightDir = normalize(-l.Rotation);
	float theta = dot(LightDir,normalize(-l.Rotation));
	if(theta > l.CutoutDist)
	{
		float diff = max(dot(norm,LightDir),0.0);
		vec3 diffuse = (diff*Mat.Diffuse)*l.Color;
		
		//Specular
		vec3 viewDir = normalize(CameraP-FragPos);
		vec3 reflectDir = reflect(-LightDir,norm);
		float spec = pow(max(dot(viewDir,reflectDir),0.0),Mat.Shininess);
		vec3 specular = (Mat.Specular*spec)*l.Color;
		
		return (Ambient+diffuse+specular)* BaseTex.xyz;
	}
	else
	{
		return Ambient;
	}
}


void main()
{	
	vec4 BaseEl = texture2D(Base,UV);
	vec4 NormalEl = texture2D(Normal,UV);
	vec4 SpecularEl = texture2D(Specular,UV);
	
	vec3 result;
	
	for(int i=0; i<32;i++)
	{
		if(Lights[i].LightType == 0)
			result += ProccessDirectionalLight(Lights[i],BaseEl,NormalEl,SpecularEl);
		else if(Lights[i].LightType == 1)
			result += ProccessPointLight(Lights[i],BaseEl,NormalEl,SpecularEl);
		else if(Lights[i].LightType == 2)
			result += ProccessSpotLight(Lights[i],BaseEl,NormalEl,SpecularEl);
	}
	

	gl_FragColor = vec4(result,1.0);
} 