#pragma once
#include "common.h"

namespace CMEngine {

	class VertexBufferLayout {
	public:
		struct VertexBufferElement {
			uint32_t type;
			uint32_t count;
			uint8_t normalized;

			static uint32_t GetSizeOfType(uint32_t type) {
				switch (type) {
				case GL_FLOAT: return sizeof(GLfloat);
				case GL_UNSIGNED_INT: return sizeof(GLuint);
				case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
				}
				return 0;
			}

		};

		VertexBufferLayout()
			: m_Stride(0) {

		}

		template<typename T>
		void Push(uint32_t count) {
			//static_assert(false); // this crap doesn't work on c++20, works properly on c++17 though
		}

		template<>
		void Push<float>(uint32_t count) {
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<uint32_t>(uint32_t count) {
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<uint8_t>(uint32_t count) {
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const {
			return m_Elements;
		}

		inline uint32_t GetStride() const {
			return m_Stride;
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;
	};


}
