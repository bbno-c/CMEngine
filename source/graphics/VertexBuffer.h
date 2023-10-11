#pragma once
#include "VertexArray.h"

namespace CMEngine {
	class VertexBuffer {
	public:
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;


		void Bind() const;
		void Unbind() const;
		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	private:
		uint32_t m_ID;
		BufferLayout m_Layout;
	};
}