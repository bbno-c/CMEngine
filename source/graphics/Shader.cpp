#include "common.h"
#include "Shader.h"

namespace CMEngine {

	Shader::Shader(std::string vertex_source_path, std::string fragment_source_path) {
		uint32_t vertex, fragment;
		vertex = loadShader(GL_VERTEX_SHADER, vertex_source_path.c_str());
		fragment = loadShader(GL_FRAGMENT_SHADER, fragment_source_path.c_str());
		{
			Timer t;
			m_ID = createShaderProgram(vertex, fragment);
			std::cout << "Shader creation took " << t.ElapsedMillis() << " ms";
		}
	}
	Shader::~Shader() {
		glDeleteProgram(m_ID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	uint32_t Shader::loadShader(const GLenum type, const char* file_path) {
		// Read the shader source code from the file
		std::string code;
		std::ifstream shader_file;
		shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			VirtualFileSystem& vfs = VirtualFileSystem::GetInstance("Assets/");
			if (!vfs.LoadFile(file_path, code)) {
				throw std::runtime_error("vfs.LoadFile");
			}
		}
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* shader_code = code.c_str();

		// Compile shader
		const uint32_t shader = glCreateShader(type);
		glShaderSource(shader, 1, &shader_code, NULL);
		glCompileShader(shader);

		// Check for shader compile errors
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info_log[512];
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		return shader;
	}
	uint32_t Shader::createShaderProgram(const uint32_t vertex_shader, const uint32_t fragment_shader) {
		const uint32_t program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// Check for linking errors
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char info_log[512];
			glGetProgramInfoLog(program, 512, NULL, info_log);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
		}

		// After linking, shaders can be deleted
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}