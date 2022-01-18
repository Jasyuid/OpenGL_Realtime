#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 camera;
uniform mat4 model;

void main()
{
	gl_Position = camera * model * vec4(vertexPos, 1.0);
};

#SHADER FRAGMENT===============================================================
#version 330 core

void main()
{
};