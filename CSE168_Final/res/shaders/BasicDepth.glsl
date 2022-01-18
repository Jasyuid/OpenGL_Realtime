#SHADER VERTEX=================================================================
#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(vertexPos, 1.0);
};


#SHADER GEOMETRY===============================================================
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 fragPos;

uniform mat4 shadow_matrices[6];

void main()
{
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; ++i)
		{
			fragPos = gl_in[i].gl_Position;
			gl_Position = shadow_matrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
};


#SHADER FRAGMENT===============================================================
#version 330 core

in vec4 fragPos;

uniform vec3 light_pos;
uniform float far_plane;

void main()
{
	float lightDist = length(fragPos.xyz - light_pos);

	lightDist = lightDist / far_plane;

	gl_FragDepth = lightDist;
};