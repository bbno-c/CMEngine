#include "VertexBuffer.h"
#include "Renderer.h"

namespace CMEngine {

	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_RendererID);
	}

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_RendererID != 0) {
			glDeleteBuffers(1, &m_RendererID);
		}
	}

	void VertexBuffer::AddBufferData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}