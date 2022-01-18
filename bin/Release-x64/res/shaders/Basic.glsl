#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

out vec3 fragColor;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * vec4(vertexPos, 1.0);
	fragColor = vertexColor;
	fragPos = vec3(model * vec4(vertexPos, 1.0));
};


#SHADER FRAGMENT===============================================================
#version 330 core

layout(location = 0) out vec4 color;

in vec3 fragColor;
in vec3 fragPos;

void main()
{
	color = vec4(fragColor, 1.0);
};