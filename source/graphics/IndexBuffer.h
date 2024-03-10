#pragma once

namespace CMEngine {

	class IndexBuffer {
	public:
		IndexBuffer();
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer&& other) noexcept : m_Count{ std::exchange(other.m_Count, 0) }, m_RendererID{ std::exchange(other.m_RendererID, 0) } {}
		IndexBuffer& operator=(IndexBuffer&& other) noexcept
		{
			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Count = std::exchange(other.m_Count, 0);
			return *this;
		}

		void Bind() const;
		void Unbind() const;
		inline uint32_t GetCount() const { return m_Count; }
		void AddBufferData(uint32_t* indices, uint32_t count);

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}