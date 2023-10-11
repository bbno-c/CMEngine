#include "VertexBuffer.h"
#include "Renderer.h"

namespace CMEngine {

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		GL_CALL(glGenBuffers(1, &m_ID));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_ID));
	}

	void VertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	}

	void VertexBuffer::Unbind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}