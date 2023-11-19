#include "VertexBuffer.h"
#include "Renderer.h"

namespace CMEngine {

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		(glGenBuffers(1, &m_ID));
		(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		(glDeleteBuffers(1, &m_ID));
	}

	void VertexBuffer::Bind() const
	{
		(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	}

	void VertexBuffer::Unbind() const
	{
		(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}