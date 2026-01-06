#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/asset_service.hpp"
#include "render/render_pch.hpp"
#include "core/utils/string_hasher.hpp"
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

		enum class ShaderType {
			Basic
		};

		class BasicShader {
		public:
			
			GLuint program;

			struct Uniforms {
				GLint uModel;
			} uniforms;

			void Bind() const {
				glUseProgram(program);
			}

		};

		struct PerObjectUniforms {
			GLuint uModel;
		};

		struct ShaderUniforms {
			PerObjectUniforms object;
		};

		class ShaderManager {
		public:

			BasicShader basic_shader;

			[[nodiscard]] ShaderHandle CreateShader(std::string_view, std::string_view);

			GLuint GetProgram(ShaderHandle handle) {
				return m_shader_handles[handle];
			}

			void SetMatrix4fv(ShaderHandle handle, std::string_view location, const glm::mat4& matrix);

			void SetVector3fv(ShaderHandle handle, std::string_view location, const glm::vec3& vector);

			void Init() {
				CreateBasicShader();
				LOG_INIT_OK("Shader manager initialization");
			}

		private:

			void CreateBasicShader() {

				GLuint vert_shader = CreateVertexShader("basic.vert");
				GLuint frag_shader = CreateFragmentShader("basic.frag");

				basic_shader.program = glCreateProgram();

				glAttachShader(basic_shader.program, vert_shader);
				glAttachShader(basic_shader.program, frag_shader);
				LinkProgram(basic_shader.program);

				glDeleteShader(vert_shader);
				glDeleteShader(frag_shader);

				basic_shader.Bind();

				basic_shader.uniforms.uModel = glGetUniformLocation(basic_shader.program, "uModel");

			}
			
			void LinkProgram(GLuint program);
			void CompileShader(GLuint shader);

			GLuint CreateVertexShader(std::string file) {

				std::string vert_file = lum::AssetService::LoadShader(file);
				const char* vert_c_str = vert_file.c_str();

				GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vert_shader, 1, &vert_c_str, nullptr);
				CompileShader(vert_shader);

				return vert_shader;

			}

			GLuint CreateFragmentShader(std::string file) {

				std::string frag_file = lum::AssetService::LoadShader(file);
				const char* frag_c_str = frag_file.c_str();

				GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(frag_shader, 1, &frag_c_str, nullptr);
				CompileShader(frag_shader);

				return frag_shader;

			}

			cstd::handle_pool<GLuint, ShaderHandle> m_shader_handles{ 10 };

		};


	}
}