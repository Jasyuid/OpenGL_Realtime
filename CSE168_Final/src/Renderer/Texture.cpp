#include "oglpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture(const std::string& filepath, bool flip, bool hdr)
	: m_RendererID(0), m_FilePath(filepath), m_LocalBuffer(nullptr), 
	m_HDRBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	if (hdr)
		LoadHDR(filepath, flip);
	// Load texture
	stbi_set_flip_vertically_on_load(flip);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (!m_LocalBuffer)
	{
		LOG_WARN("Unable to find {0}!", filepath);
		return;
	}

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::LoadHDR(const std::string& filepath, bool flip)
{
	// Load texture
	stbi_set_flip_vertically_on_load(flip);
	m_HDRBuffer = stbi_loadf(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	if (!m_HDRBuffer)
	{
		LOG_WARN("Unable to find {0}!", filepath);
		return;
	}

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, m_HDRBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_HDRBuffer)
		stbi_image_free(m_HDRBuffer);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
