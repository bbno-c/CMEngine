#include "common.h"
#include "Texture.h"
#include "../../external/stb/stb_image.h"

namespace CMEngine {
	Texture::Texture(const std::string& path)
		: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_nrComponents(0)
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_nrComponents, 4);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer) {
			stbi_image_free(m_LocalBuffer);
		}
	}

	Texture::Texture(const char* path, const std::string& directory, const std::string& type, bool flipVertically)
		: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_nrComponents(0), m_Type(type)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		stbi_set_flip_vertically_on_load(flipVertically);

		glGenTextures(1, &m_RendererID);

		unsigned char* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_nrComponents, 0);
		if (data)
		{
			GLenum format = GL_RGBA8;
			if (m_nrComponents == 1)
				format = GL_RED;
			else if (m_nrComponents == 3)
				format = GL_RGB;
			else if (m_nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
	}

	Texture::~Texture()
	{
		//glDeleteTextures(1, &m_RendererID);
	}
	void Texture::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}
	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}