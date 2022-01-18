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

uniform samplerCube shadow_maps[MAX_LIGHTS];
uniform float far_plane;

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

float ShadowCalculation(vec3 fragPos, vec3 normal, vec3 light_pos, vec3 light_dir, samplerCube shadow_map)
{
	vec3 lightDist = fragPos - light_pos;

	float currentDepth = length(lightDist);
	
	vec3 sampleOffsets[20] = vec3[]
	(
		vec3( 1, 1, 1), vec3( 1,-1, 1), vec3(-1,-1, 1), vec3(-1, 1, 1),
		vec3( 1, 1,-1), vec3( 1,-1,-1), vec3(-1,-1,-1), vec3(-1, 1,-1),
		vec3( 1, 1, 0), vec3( 1,-1, 0), vec3(-1,-1, 0), vec3(-1, 1, 0),
		vec3( 1, 0, 0), vec3(-1, 0, 1), vec3( 1, 0,-1), vec3(-1, 0,-1),
		vec3( 0, 1, 1), vec3( 0,-1, 1), vec3( 0,-1,-1), vec3( 0, 1,-1)
	);

	float bias = 0.01;
	float shadow = 0.0;
	float samples = 20;
	float viewDist = length(view_pos - fragPos);
	float diskRadius = (1.0 + (viewDist / far_plane)) / 25.0;
	for(int i = 0; i < samples; i++)
	{
		float closestDepth = texture(shadow_map, lightDist + sampleOffsets[i] * diskRadius).r * far_plane;
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);

	return shadow;
}

void main()
{
	vec3 diff = pow(texture(material.texture_diffuse, texCoord).rgb, vec3(2.2));
	float spec = texture(material.texture_specular, texCoord).r;
	float rough = texture(material.texture_roughness, texCoord).r + material.rough_add;

	//vec3 normal = normalize(fragNorm);
	vec3 normal = CalculateNormal();
	vec3 viewDir = normalize(view_pos - fragPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, diff, spec);

	vec3 L0 = vec3(0.0);
	for(int i = 0; i < light_num; i++)
	{
		vec3 lightDir = normalize(light[i].position - fragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float lightDist = length(light[i].position - fragPos);
		float atten = 1.0 / (0.01 * (lightDist * lightDist));
		vec3 radiance = light[i].color * atten;

		float D = CalculateD(normal, halfwayDir, rough);
		float G = CalculateG(normal, viewDir, lightDir, rough);
		vec3 F = CalculateF(max(dot(halfwayDir, viewDir), 0.0), F0);

		vec3 specular = (D * G * F) / (4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001);

		vec3 kd = (vec3(1.0) - F) * (1.0 - spec);
		//vec3 ks = F;

		//Shadow
		float shadow = 0.0;
		shadow = ShadowCalculation(fragPos, normal, light[i].position, lightDir, shadow_maps[i]);

		L0 += (1.0 - shadow) * (kd * diff / PI + specular) * radiance * max(dot(normal, lightDir), 0.0);
	};

	vec3 ambient = vec3(0.03) * diff;

	vec3 result = ambient + L0;

	result = result / (result + vec3(1.0));
	result = pow(result, vec3(1.0/2.2));
	
	color = vec4(result, 1.0);
};