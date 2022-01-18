#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 vertexTC;

out vec2 texCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vertexPos, 1.0, 1.0);
	texCoords = vertexTC;
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
	color = vec4(textColor, 1.0) * sampled;
};