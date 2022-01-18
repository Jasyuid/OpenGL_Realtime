#pragma once

// Vertex buffer object
class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, bool dyn = false);
	~VertexBuffer();

	void SubData(const void* data, unsigned int size); // Update buffer data

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID; // Buffer ID
	unsigned int m_Size; // Buffer size
	bool m_Dynamic; // Buffer dynamic
};

// Index buffer object
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count, bool dyn = false);
	~IndexBuffer();

	void SubData(const void* data, unsigned int count); // Update buffer data

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID; // Buffer ID
	unsigned int m_Count; // Number of indices
	bool m_Dynamic; // Buffer dynamic
};