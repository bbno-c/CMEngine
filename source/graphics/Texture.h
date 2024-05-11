#pragma once
#include "Renderer.h"

namespace CMEngine {
	class Texture {
	public:
		Texture() {};
		Texture(const char* path, const std::string& directory, const std::string& type, bool flipVertically = true);
		Texture(const std::string& path);
		Texture(const std::string& path, int32_t texture_wrap);
		~Texture();

		void Bind(uint32_t slot = 0) const;
		void Unbind();

		inline int32_t GetWidth() const { return m_Width; }
		inline int32_t GetHeight() const { return m_Height; }
		inline uint32_t GetID() const { return m_RendererID; };
		inline std::string GetPath() const { return m_FilePath; };
		inline std::string GetType() const { return m_Type; };
		inline void SetType(const std::string& type) { m_Type = type; };

	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		uint8_t* m_LocalBuffer;
		int32_t m_Width, m_Height, m_nrComponents;
		std::string m_Type;

	};
}