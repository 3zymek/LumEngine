#include "render/core/shader.hpp"
namespace lum {
	namespace render {
		ShaderHandle ShaderManager::CreateShader(std::string_view vert_file_name, std::string_view frag_file_name) {

			std::string vert_file = lum::AssetService::LoadShader(vert_file_name);
			std::string frag_file = lum::AssetService::LoadShader(frag_file_name);

			const char* vert_c_str = vert_file.c_str();
			const char* frag_c_str = frag_file.c_str();

			GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vert_shader, 1, &vert_c_str, nullptr);
			CompileShader(vert_shader);

			GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(frag_shader, 1, &frag_c_str, nullptr);
			CompileShader(frag_shader);

			GLuint shader_program = glCreateProgram();

			glAttachShader(shader_program, vert_shader);
			glAttachShader(shader_program, frag_shader);
			glLinkProgram(shader_program);

			glDeleteShader(vert_shader);
			glDeleteShader(frag_shader);

			return m_shader_handles.CreateHandle(shader_program);

		}

		void ShaderManager::SetMatrix4fv(ShaderHandle handle, std::string_view location, const glm::mat4& matrix) {

			GLint u_location = glGetUniformLocation(m_shader_handles[handle], location.data());
			glUniformMatrix4fv(u_location, 1, GL_FALSE, glm::value_ptr(matrix));

		}

		void ShaderManager::SetVector3fv(ShaderHandle handle, std::string_view location, const glm::vec3& vector) {

			GLint u_location = glGetUniformLocation(m_shader_handles[handle], location.data());
			glUniform3fv(u_location, 1, glm::value_ptr(vector));

		}

		void ShaderManager::LinkProgram(GLuint program) {
			glLinkProgram(program);
			int success;
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				GLchar buff[1024];
				glGetProgramInfoLog(program, 1024, nullptr, buff);
				LOG_ERROR(std::string("Shader program linking failed: ") + buff);
			}
		}
		void ShaderManager::CompileShader(GLuint shader) {

			glCompileShader(shader);

			GLint success;
			GLchar buff[1048];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, sizeof(buff), nullptr, buff);
				LOG_ERROR(buff);
			}

		}

	}
}