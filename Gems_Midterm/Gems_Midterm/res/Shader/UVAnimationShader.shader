#vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 vertexColor;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 VertexColor;

uniform vec3 textureTiling;  //x, y

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;
uniform float time;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
	//TexCoord = texCoord;
	
	TexCoord = vec2(texCoord.x * textureTiling.x, texCoord.y * textureTiling.y) + vec2(0.1 * time, 0.2 * time);
	
	vec4 worlNormal = inverseModel * vec4(normal, 1.0f);
	Normal = normalize(worlNormal.xyz);
	//Normal = normal;
	
	FragPos = vec3(model * vec4(position, 1.0f));
	VertexColor = vertexColor;
};


#fragment

#version 330 core

struct Material
{
	vec4 baseColor;
	vec4 ambientColor;
	float specularValue;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	vec4 baseColor;
	vec4 ambientColor;
	vec4 atten;								// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 type_innerAngle_outerAngle_w;		// x = lightType, y = inner angle, z = outer angle, w = TBD
											// 0 = pointlight
											// 1 = spot light
											// 2 = directional light
	vec4 param2;							// x = 0 for off, 1 for on
};

out vec4 color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 VertexColor;

uniform Material material;

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;

uniform Light[NUMBEROFLIGHTS] lights;

//uniform vec3 ambient_Specular;
//uniform vec3 lightColor;
//uniform vec3 lightPos;

uniform vec3 viewPos;

//OPAQUE = 1,
//ALPHA_BLEND = 2,
//ALPHA_CUTOUT = 3

uniform vec3 alphaCutOut;  //ShaderBlendMode //ObjectBlendMode , alphaCutoffThreshold


uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_opacity;

vec4 CalcDirLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 CalculateLightContrib(vec3 normal, vec3 fragPos, vec3 viewDir );


void main()
{
	//color = mix(texture(texture_diffuse1, TexCoord), texture(overlayTex, TexCoord), 0f);
	vec3 normalizedNormal = normalize(Normal.xyz);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec4 result = CalculateLightContrib(normalizedNormal,FragPos,viewDir);
	
    //color = vec4(specular, 1.0);
	
	
	
	
	//color = vec4(diffuseColor.w);
	
	if(alphaCutOut.x == 1)
	{
		result.w = 1.0;
	}
	else if(alphaCutOut.x == 3)
	{
		if(result.w < alphaCutOut.z)
		{
			discard;
		}
	}
	
	
	color = result;
	
};


vec4 CalculateLightContrib(vec3 normal, vec3 fragPos, vec3 viewDir )
{

	vec4 result = vec4(0.0);
	
	//Tex Color
	vec4 diffuseColor = texture(texture_diffuse, TexCoord);
	
	
	vec4 texColor = diffuseColor * VertexColor * material.baseColor;
	
	for(int i = 0; i < NUMBEROFLIGHTS; i++)
	{
		if(lights[i].param2.x == 0.0)
		{
			continue; 			//Light is off
		}
		
	
		//Ambient
		vec4 ambientColor = lights[i].ambientColor * material.ambientColor;
		ambientColor *= texColor;
		
		int type = int(lights[i].type_innerAngle_outerAngle_w.x);
		
		if(type == POINT_LIGHT_TYPE)
		{
				result += CalcPointLight(lights[i], texColor, ambientColor, normal,FragPos, viewDir);
		}
		else if(type == SPOT_LIGHT_TYPE)
		{
			result += CalcSpotLight(lights[i], texColor, ambientColor, normal, FragPos, viewDir);
		}
		else if(type == DIRECTIONAL_LIGHT_TYPE)
		{
			result += CalcDirLight(lights[i], texColor, ambientColor, normal, viewDir);
		}
	}
	
	if(alphaCutOut.y == 1)
	{
		result.w = texture(texture_opacity, TexCoord).r;
	}
	else
	{
		result.w =  material.baseColor.w;
	}
	
	
	return result;
}

vec4 CalcDirLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 viewDir)
{
	//Diffuse
	
	vec3 lightDir = normalize(- light.direction);
	float diffValue = max(dot(normal,lightDir),0.0);
	
	vec4 diffuse = diffValue * light.baseColor;
	diffuse *= texColor;
	
	//Specular
	vec4 specularColor = texture(texture_specular, TexCoord);
	vec3 reflectDir = reflect(-lightDir,normal);
	
	float specularValue = pow(max(dot(reflectDir, viewDir),0.0),material.shininess);
	//vec4 specular = specularValue * material.specularValue * light.baseColor * (specularColor.r);
	vec4 specular = vec4(specularValue * material.specularValue * light.baseColor.xyz * (specularColor.r),light.baseColor.w);
	
	return (ambientColor + diffuse + specular) ;
}


vec4 CalcPointLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Attenuation 
	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	
	float _distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * _distance + light.atten.z * (_distance * _distance));   
	
	//Ambient
	ambientColor.xyz *= attenuation;
	
	//Diffuse
	
	vec3 lightDir = normalize(light.position - fragPos);
	float diffValue = max(dot(normal,lightDir),0.0);
	vec4 diffuse = diffValue * light.baseColor;
	diffuse *= texColor * attenuation;
	
	//Specular
	vec4 specularColor = texture(texture_specular, TexCoord);
	vec3 reflectDir = reflect(-lightDir,normal);
	
	float specularValue = pow(max(dot(reflectDir, viewDir),0.0),material.shininess);
	vec4 specular = specularValue * material.specularValue * light.baseColor * (specularColor.r);
	specular *= attenuation;
	
	return (ambientColor + diffuse + specular) ;
}


vec4 CalcSpotLight(Light light, vec4 texColor, vec4 ambientColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	
	//Attenuation 
	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	
	vec3 lightDir = normalize(light.position - fragPos);
	
	float _distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * _distance + light.atten.z * (_distance * _distance));   
	
	// spotlight intensity
	float outerConeAngleCos = cos(radians(light.type_innerAngle_outerAngle_w.z));
	float innerConeAngleCos = cos(radians(light.type_innerAngle_outerAngle_w.y));
	
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = innerConeAngleCos - outerConeAngleCos;
    float intensity = clamp((theta - outerConeAngleCos) / epsilon, 0.0, 1.0);
	
	//Ambient
	ambientColor *= attenuation * intensity;
	
	//Diffuse
	
	float diffValue = max(dot(normal,lightDir),0.0);
	vec4 diffuse = diffValue * light.baseColor;
	diffuse *= texColor * attenuation * intensity;
	
	
	//Specular
	vec4 specularColor = texture(texture_specular, TexCoord);
	vec3 reflectDir = reflect(-lightDir,normal);
	
	float specularValue = pow(max(dot(reflectDir, viewDir),0.0),material.shininess);
	vec4 specular = specularValue * material.specularValue * light.baseColor * (specularColor.r);
	specular *= attenuation * intensity;
	
	return (ambientColor + diffuse + specular) ;
}