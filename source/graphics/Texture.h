#pragma once
#include "Renderer.h"

namespace CMEngine {
	class Texture {
	public:
		Texture(const std::string& path);
		~Texture();

		void Bind(uint32_t slot = 0) const;
		void Unbind();

		inline int32_t GetWidth() const { return m_Width; }
		inline int32_t GetHeight() const { return m_Height; }
		inline uint32_t GetID() const { return m_RendererID; };

	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		uint8_t* m_LocalBuffer;
		int32_t m_Width, m_Height, m_BPP;

	};
}