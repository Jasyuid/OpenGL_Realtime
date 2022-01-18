#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTC;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * vec4(vertexPos, 1.0);
	fragPos = vec3(model * vec4(vertexPos, 1.0));
	normal = vertexNorm;
	texCoord = vertexTC;
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Material 
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
};

uniform Material material;

void main()
{
	// Ambient Lighting
	vec3 ambient = 0.1 * vec3(texture(material.texture_diffuse, texCoord));

	// Diffuse Lighting
	vec3 diffuse = vec3(texture(material.texture_diffuse, texCoord));
	
	// Specular Lighting
	vec3 specular = vec3(texture(material.texture_specular, texCoord));

	vec3 result = diffuse;
	color = vec4(result, 1.0);
};