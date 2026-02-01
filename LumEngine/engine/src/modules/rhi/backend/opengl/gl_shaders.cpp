#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	ShaderHandle GLDevice::CreateShader(const ShaderDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mShaders.dense_size() < skMaxShaders,
			"Max shaders reached",
			ShaderHandle{}
		);

		std::string&& vfile = AssetService::load_internal_shader(desc.vertex_source);
		ccharptr vcstr = vfile.c_str();

		std::string&& ffile = AssetService::load_internal_shader(desc.fragment_source);
		ccharptr fcstr = ffile.c_str();

		Shader shader;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vcstr, nullptr);
		glShaderSource(fshader, 1, &fcstr, nullptr);

		glCompileShader(vshader);
		GLint success;
		glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char buff[2048];
			glGetShaderInfoLog(vshader, 2048, nullptr, buff);
			LUM_LOG_ERROR(buff);
		}


		glCompileShader(fshader);
		glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char buff[2048];
			glGetShaderInfoLog(fshader, 2048, nullptr, buff);
			LUM_LOG_ERROR(buff);
		}

		shader.handle = glCreateProgram();

		glAttachShader(shader.handle, vshader);
		glAttachShader(shader.handle, fshader);

		glDeleteShader(vshader);
		glDeleteShader(fshader);

		glLinkProgram(shader.handle);

		LUM_LOG_INFO(std::format("Created shader {}, {}", desc.vertex_source, desc.fragment_source));
		return mShaders.create_handle(std::move(shader));
	}
	void GLDevice::BindShader(const ShaderHandle& shader) {

		LUM_HOTCHK_RETURN_VOID(mShaders.exist(shader), "Shader doesn't exists");

		glUseProgram(mShaders[shader].handle);

	}
	void GLDevice::DeleteShader(ShaderHandle& shader) {

	}
	void GLDevice::SetMat4(const ShaderHandle& shader, ccharptr location, const glm::mat4& mat) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GLDevice::Setf(const ShaderHandle& shader, ccharptr location, float32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1f(loc, value);

	}
	void GLDevice::Seti(const ShaderHandle& shader, ccharptr location, int32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1i(loc, value);

	}
	void GLDevice::SetVec4(const ShaderHandle& shader, ccharptr location, const glm::vec4& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec3(const ShaderHandle& shader, ccharptr location, const glm::vec3& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec2(const ShaderHandle& shader, ccharptr location, const glm::vec2& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, std::format("Couldn't localize uniform named {}", location));
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}
}