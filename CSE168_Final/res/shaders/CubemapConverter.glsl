#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTC;

out vec3 fragPos;

uniform mat4 camera;

void main()
{
	fragPos = vertexPos;
	gl_Position = camera * vec4(vertexPos, 1.0);
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec3 fragPos;

uniform sampler2D rectMap;

void main()
{
	vec3 pos = normalize(fragPos);
	vec2 uv = vec2(atan(pos.z, pos.x), asin(pos.y));
	uv *= vec2(0.1591, 0.3183);
	uv += 0.5;
	vec3 result = texture(rectMap, uv).rgb;
	//result = vec3(1.0, 0.0, 1.0);

	color = vec4(result, 1.0);
};