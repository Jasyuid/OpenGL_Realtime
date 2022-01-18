#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTC;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * vec4(vertexPos, 1.0);
	fragPos = vec3(model * vec4(vertexPos, 1.0));

	fragNorm = transpose(inverse(mat3(model))) * vertexNorm;
	
	texCoord = vertexTC;
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 texCoord;

struct Material 
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
};

struct Light
{
	vec3 position;
	vec3 color;
};

#define MAX_LIGHTS 4

uniform Material material;
uniform Light light[MAX_LIGHTS];
uniform int light_num;
uniform vec3 view_pos;

void main()
{
	vec3 baseColor = vec3(texture(material.texture_diffuse, texCoord));
	vec3 specColor = vec3(texture(material.texture_specular, texCoord));

	// Ambient Lighting
	vec3 ambient = 0.05 * baseColor;
	vec3 result = ambient;

	for(int i = 0; i < light_num; i++)
	{
		// Diffuse Lighting
		vec3 lightDir = normalize(light[i].position - fragPos);
		vec3 normal = normalize(fragNorm);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = light[i].color * diff * baseColor;
	
		// Specular Lighting
		vec3 viewDir = normalize(view_pos - fragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
		vec3 specular = light[i].color * spec * specColor;

		// Attenuation
		float distance = length(light[i].position - fragPos);
		float atten = 1.0 / (0.1*(distance * distance));
		diffuse *= atten;
		specular *= atten;

		result += diffuse + specular;
	};

	result = pow(result, vec3(1.0/2.2));
	color = vec4(result, 1.0);
};