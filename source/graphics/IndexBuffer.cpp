#include "IndexBuffer.h"
#include "Renderer.h"

namespace CMEngine {

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		GL_CALL(glGenBuffers(1, &m_ID));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_ID));
	}

	void IndexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	}

	void IndexBuffer::Unbind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}