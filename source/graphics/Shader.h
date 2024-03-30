#pragma once

namespace CMEngine {

	// TODO add uniform location caching
	class Shader {
	public:
		Shader(std::string vertex_source_path, std::string fragment_source_path);
		Shader(std::string vertex_source_path, std::string fragment_source_path, std::string geometry_source_path);
		~Shader();
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;

		void Bind() const;
		void Unbind() const;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


    private:
		uint32_t loadShader(const GLenum type, const char* file_path);
		uint32_t createShaderProgram(const uint32_t vertex_shader, const uint32_t fragment_shader);
		uint32_t createShaderProgram(const uint32_t vertex_shader, const uint32_t fragment_shader, const uint32_t geometry_shader);
	private:
		uint32_t m_ID;
	};
}