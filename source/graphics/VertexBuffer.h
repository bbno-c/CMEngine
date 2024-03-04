#pragma once
//#include "VertexArray.h"
#include "VertexBufferLayout.h"

namespace CMEngine {

	class VertexBuffer {
	public:
		VertexBuffer();
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
		const VertexBufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }
		void AddBufferData(const void* data, uint32_t size);

	private:
		uint32_t m_ID;
		VertexBufferLayout m_Layout;
	};


}