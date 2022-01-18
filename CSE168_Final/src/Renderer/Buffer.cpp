#include "oglpch.h"
#include "Buffer.h"

#include <glad/glad.h>

///////////////////////////////////////////////////////////////////////////////
// VERTEX BUFFER
///////////////////////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool dyn)
	:m_RendererID(0), m_Size(size), m_Dynamic(dyn)
{
	glGenBuffers(1, &m_RendererID); // Generate a new buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // Bind the new buffer
	// Send vertices to buffer
	if (dyn)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); // Buffer being updated
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // Buffer that is static
	}
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SubData(const void* data, unsigned int size)
{
	// Warn if buffer is not dynamic
	if (!m_Dynamic)
		LOG_WARN("Updating data in static buffer!");
	// Warn if data is too large for buffer
	if (size > m_Size)
		LOG_WARN("New vertex data larger than buffer size! ({0} > {1})", size, m_Size);

	// Update buffer data
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data); //TODO: Maybe change this to mapBuffer
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

///////////////////////////////////////////////////////////////////////////////
// INDEX BUFFER
///////////////////////////////////////////////////////////////////////////////
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, bool dyn)
	:m_RendererID(0), m_Count(count), m_Dynamic(dyn)
{
	glGenBuffers(1, &m_RendererID); // Generate a new buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // Bind the new buffer
	// TODO: Possibly change unsigned int in IndexBuffer
	if (dyn) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::SubData(const void* data, unsigned int count)
{
	// Warn if buffer is not dynamic
	if (!m_Dynamic)
		LOG_WARN("Updating data in static buffer!");
	// Warn if data is too large for buffer
	if (count > m_Count)
		LOG_WARN("New index data count than buffer count! ({0} > {1})", count, m_Count);

	// Update buffer data
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data); //TODO: Maybe change this to mapBuffer
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
