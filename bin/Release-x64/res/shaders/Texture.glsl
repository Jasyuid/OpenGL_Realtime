#SHADER VERTEX
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTC;
layout(location = 2) in vec3 vertexNormal;

out vec2 texCoord;
out vec3 fragmentPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * vec4(vertexPosition, 1.0);
	texCoord = vertexTC;
	fragmentPos = vec3(model * vec4(vertexPosition, 1.0));
	normal = vertexNormal;
};


#SHADER FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoord;
in vec3 fragmentPos;
in vec3 normal;

struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shine;
};

struct Light
{
	vec3 position;

	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	// Ambient Lighting
	vec3 ambient = 0.1 * vec3(texture(material.diffuse, texCoord));

	// Diffuse Lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragmentPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	
	// Specular Lighting
	vec3 viewDir = normalize(viewPos - fragmentPos);
	//vec3 reflectDir = normalize(reflect(-lightDir, norm));
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shine);
	vec3 specular = light.specular * spec * material.specular;


	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
};