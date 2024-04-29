#pragma once
#include "VertexBufferLayout.h"

namespace CMEngine {

	class VertexBuffer {
	public:
		VertexBuffer();
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept : m_RendererID{ std::exchange(other.m_RendererID, 0) } {}
		VertexBuffer& operator=(VertexBuffer&& other) noexcept
		{
			m_RendererID = std::exchange(other.m_RendererID, 0);
			return *this;
		}

		void Bind() const;
		void Unbind() const;
		const VertexBufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }
		void AddBufferData(const void* data, uint32_t size);

	private:
		uint32_t m_RendererID;
		VertexBufferLayout m_Layout;
	};


}