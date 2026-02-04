#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	ShaderHandle GLDevice::CreateShader(const ShaderDescriptor& desc) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mShaders.dense_size() < skMaxShaders,
			ShaderHandle{},
			"Max shaders reached"
		);

		bool success{};
		std::string&& vfile = AssetService::load_internal_shader(desc.vertexSource, success);
		ccharptr vcstr = vfile.c_str();

		if (!success) {
			LUM_LOG_ERROR("Couldn't load shader %s", desc.vertexSource);
			return ShaderHandle{};
		}

		std::string&& ffile = AssetService::load_internal_shader(desc.fragmentSource, success);
		ccharptr fcstr = ffile.c_str();

		if (!success) {
			LUM_LOG_ERROR("Couldn't load shader %s", desc.fragmentSource);
			return ShaderHandle{};
		}

		Shader shader;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vcstr, nullptr);
		glShaderSource(fshader, 1, &fcstr, nullptr);

		if (!_CompileShader(vshader))
			return ShaderHandle{};
		if (!_CompileShader(fshader))
			return ShaderHandle{};

		shader.handle = glCreateProgram();

		glAttachShader(shader.handle, vshader);
		glAttachShader(shader.handle, fshader);

		glDeleteShader(vshader);
		glDeleteShader(fshader);

		glLinkProgram(shader.handle);

		LUM_LOG_INFO("Created shader %s - %s", desc.vertexSource, desc.fragmentSource);
		return mShaders.create_handle(std::move(shader));
	}
	void GLDevice::BindShader(const ShaderHandle& shader) {

		LUM_HOTCHK_RETURN_VOID(mShaders.exist(shader), "Shader doesn't exist");

		glUseProgram(mShaders[shader].handle);

	}
	void GLDevice::DeleteShader(ShaderHandle& shader) {

	}
	void GLDevice::SetMat4(const ShaderHandle& shader, ccharptr location, const glm::mat4& mat) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GLDevice::Setf(const ShaderHandle& shader, ccharptr location, float32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniform1f(loc, value);

	}
	void GLDevice::Seti(const ShaderHandle& shader, ccharptr location, int32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniform1i(loc, value);

	}
	void GLDevice::SetVec4(const ShaderHandle& shader, ccharptr location, const glm::vec4& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec3(const ShaderHandle& shader, ccharptr location, const glm::vec3& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec2(const ShaderHandle& shader, ccharptr location, const glm::vec2& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, "Couldn't localize uniform named {}");
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}
	
	bool GLDevice::_CompileShader(GLuint shader) noexcept {

		glCompileShader(shader);
		int32 success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			
			char buff[2048];
			glGetShaderInfoLog(shader, 2048, nullptr, buff);
			LUM_LOG_FATAL("Failed to compile shader: %s", buff);
			return false;

		}

		return true;
	}
}