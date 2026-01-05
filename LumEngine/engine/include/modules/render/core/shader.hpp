#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/asset_service.hpp"
#include "render/render_pch.hpp"
namespace lum {
	namespace render {

		namespace detail {

			using ShaderID = uint32_t;
			using Generation = uint32_t;

		}

		struct ShaderHandle {

			detail::ShaderID	id;
			detail::Generation	generation;

		};

		class ShaderManager {
		public:

			[[nodiscard]] ShaderHandle CreateShader(std::string vert_file_name, std::string frag_file_name) {

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

				return shader_handles.CreateHandle(shader_program);

			}

			GLuint GetProgram(ShaderHandle handle) {
				return shader_handles[handle];
			}

			void SetMatrix4fv(ShaderHandle handle, std::string_view location, const glm::mat4& matrix) {

				GLint u_location = glGetUniformLocation(shader_handles[handle], location.data());
				glUniformMatrix4fv(u_location, 1, GL_FALSE, glm::value_ptr(matrix));

			}

			void SetVector3fv(ShaderHandle handle, std::string_view location, const glm::vec3& vector) {

				GLint u_location = glGetUniformLocation(shader_handles[handle], location.data());
				glUniform3fv(u_location, 1, glm::value_ptr(vector));

			}

			void UseShader(ShaderHandle handle) {
				glUseProgram(shader_handles[handle]);
			}


		private:

			void CompileShader(GLuint& shader) {

				glCompileShader(shader);

				GLint success;
				GLchar buff[2048];
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, sizeof(buff), nullptr, buff);
					LOG_ERROR(buff);
				}

			}


			cstd::handle_pool<GLuint, ShaderHandle> shader_handles{ 10 };

		};

		struct Shader {

			Shader(ShaderManager& mgr, ShaderHandle h) : manager(mgr), handle(h) {}

			void SetMatrix4fv(std::string_view location, const glm::mat4& matrix) {

				manager.SetMatrix4fv(handle, location, matrix);

			}
			void SetVector3fv(std::string_view location, const glm::vec3& vector) {

				manager.SetVector3fv(handle, location, vector);

			}
			void Use() const { manager.UseShader(handle); }


		private:

			ShaderManager& manager;
			ShaderHandle handle;

		};

	}
}