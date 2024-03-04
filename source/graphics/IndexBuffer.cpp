#include "IndexBuffer.h"
#include "Renderer.h"

namespace CMEngine {

	IndexBuffer::IndexBuffer() : m_Count(0)
	{
		glGenBuffers(1, &m_ID);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		(glGenBuffers(1, &m_ID));
		(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
		(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	void IndexBuffer::AddBufferData(uint32_t* indices, uint32_t count)
	{
		m_Count = count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		(glDeleteBuffers(1, &m_ID));
	}

	void IndexBuffer::Bind() const
	{
		(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	}

	void IndexBuffer::Unbind() const
	{
		(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}