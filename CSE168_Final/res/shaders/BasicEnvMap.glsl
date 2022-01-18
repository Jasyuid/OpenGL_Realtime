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
	fragPos = vec3(model * vec4(vertexPos, 1.0));
	fragNorm = transpose(inverse(mat3(model))) * vertexNorm;
	texCoord = vertexTC;
	
	gl_Position = camera * vec4(fragPos, 1.0);
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
	sampler2D texture_normal;
	sampler2D texture_roughness;
	float rough_add;
};

uniform Material material;
uniform vec3 view_pos;

uniform samplerCube env_map;

const float PI = 3.1415926536;

vec3 CalculateNormal()
{
	vec3 tangentNormal = texture(material.texture_normal, texCoord).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(fragPos);
	vec3 Q2 = dFdy(fragPos);
	vec2 st1 = dFdx(texCoord);
	vec2 st2 = dFdy(texCoord);

	vec3 N = normalize(fragNorm);
	vec3 T = normalize(Q1*st2.t - Q2*st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float CalculateD(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float NdotH = max(dot(N, H), 0.0);
	
	float d = (NdotH*NdotH*((a*a) - 1.0) + 1.0);

	return (a*a) / (PI*d*d);
}

float GGX(float NW, float roughness)
{
	float r = roughness + 1.0;
	float k = (r*r) / 8.0;

	return NW / (NW * (1.0 - k) + k);
}

float CalculateG(vec3 normal, vec3 w0, vec3 wi, float roughness)
{
	float NV = max(dot(normal, w0), 0.0);
	float NL = max(dot(normal, wi), 0.0);

	float g1 = GGX(NL, roughness);
	float g2 = GGX(NV, roughness);

	return g1 * g2;
}

vec3 CalculateF(float cosT, vec3 spec)
{
	return spec + (1.0 - spec) * pow(max(1.0 - cosT, 0.0), 5.0);
}

void main()
{
	vec3 diff = pow(texture(material.texture_diffuse, texCoord).rgb, vec3(2.2));
	float spec = texture(material.texture_specular, texCoord).r;
	float rough = texture(material.texture_roughness, texCoord).r + material.rough_add;

	//vec3 normal = normalize(fragNorm);
	vec3 normal = CalculateNormal();
	vec3 viewDir = normalize(view_pos - fragPos);

	vec3 R = reflect(-viewDir, normal);
	vec3 env_col = texture(env_map, R).rgb;
	vec3 result = diff + 0.1*env_col;

	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0/2.2));

	color = vec4(result, 1.0);
};