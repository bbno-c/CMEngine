#pragma once

namespace CMEngine {

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
		inline uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};
}