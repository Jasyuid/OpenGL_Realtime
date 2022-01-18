#pragma once

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

#include <map>

typedef std::map<std::string, Texture*> TextureMap;

struct Material
{
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine;

	Material(glm::vec3 dif = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f),
		float s = 64)
		: diffuse(dif), specular(spec), shine(s)
	{
	}
};

struct TextureMaterial : Material
{
	TextureMap textures;
};


class Mesh
{
public:
	Mesh(const void* v_data, unsigned int v_count, const VertexBufferLayout& layout, const unsigned int* i_data, unsigned int i_count, TextureMap tex_map);
	Mesh(const void* v_data, unsigned int v_count, const VertexBufferLayout& layout, const unsigned int* i_data, unsigned int i_count, Material m);
	~Mesh();

	void Draw(Shader* shader);

private:
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;

	TextureMap m_Textures;
	Material m_Mat;
};