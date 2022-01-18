#include "oglpch.h"
#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(const void* v_data, unsigned int v_count, const VertexBufferLayout& layout, const unsigned int* i_data, unsigned int i_count, TextureMap tex_map)
	: m_VAO(), m_VBO(v_data, v_count * sizeof(float)), m_IBO(i_data, i_count), m_Textures(tex_map)
{
	m_VAO.AddBuffer(m_VBO, layout);
}

Mesh::Mesh(const void* v_data, unsigned int v_count, const VertexBufferLayout& layout, const unsigned int* i_data, unsigned int i_count, Material m)
	: m_VAO(), m_VBO(v_data, v_count * sizeof(float)), m_IBO(i_data, i_count), m_Mat(m)
{
	((TextureMaterial&)m).textures;
	m_VAO.AddBuffer(m_VBO, layout);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader* shader)
{
	unsigned int textureID = 0;
	for (const std::pair<std::string, Texture*> entry : m_Textures)
	{
		if (entry.first == "texture_diffuse")
			entry.second->Bind(textureID);
		else if (entry.first == "texture_specular")
			entry.second->Bind(textureID);
		else if (entry.first == "texture_normal")
			entry.second->Bind(textureID);
		else if (entry.first == "texture_roughness")
			entry.second->Bind(textureID);
		else
			continue;

		shader->SetUniformInt(("material." + entry.first).c_str(), textureID);
		textureID++;
	}

	m_VAO.Bind();
	m_IBO.Bind();
	glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
}
