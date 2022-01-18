#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNorm;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	gl_Position = camera * model * vec4(vertexPosition, 1);
};


#SHADER FRAGMENT===============================================================
#version 330 core

out vec4 color;

uniform vec3 light_color;

void main()
{
	color = vec4(light_color, 1.0);
};