#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTC;

out vec3 fragPos;

uniform mat4 proj;
uniform mat4 view;

void main()
{
	fragPos = vertexPos;

	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = proj * rotView * vec4(fragPos, 1.0);
	gl_Position = clipPos.xyww;
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec3 fragPos;

uniform samplerCube env_map;

void main()
{
	vec3 envColor = texture(env_map, fragPos).rgb;

	//envColor = envColor / (envColor + vec3(1.0));
	//envColor = pow(envColor, vec3(1.0/2.2));

	color = vec4(envColor, 1.0);
};