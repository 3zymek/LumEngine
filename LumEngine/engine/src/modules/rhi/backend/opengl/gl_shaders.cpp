// ************************************
// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Shaders implementation for OpenGL RHI
// ************************************

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	ShaderHandle GLDevice::CreateShader ( const ShaderDescriptor& desc ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mShaders.dense_size() < skMaxShaders,
			LUM_SEV_WARN,
			ShaderHandle{},
			"Max shaders reached"
		);

		bool success;

		String&& vfile = AssetService::LoadInternalShader ( desc.vertexSource, success );
		if (!success) {
			LUM_LOG_ERROR("Couldn't load shader %s", desc.vertexSource);
			return ShaderHandle{};
		}

		String&& ffile = AssetService::LoadInternalShader ( desc.fragmentSource, success );
		if (!success) {
			LUM_LOG_ERROR("Couldn't load shader %s", desc.fragmentSource);
			return ShaderHandle{};
		}

		ccharptr vcstr = vfile.c_str();
		ccharptr fcstr = ffile.c_str();

		Shader shader;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vcstr, nullptr);
		glShaderSource(fshader, 1, &fcstr, nullptr);

		if (!compile_shader( vshader ))
			return ShaderHandle{};
		if (!compile_shader( fshader ))
			return ShaderHandle{};

		shader.handle = glCreateProgram();

		glAttachShader(shader.handle, vshader);
		glAttachShader(shader.handle, fshader);
		
		glDeleteShader(vshader);
		glDeleteShader(fshader);

		link_program(shader.handle);

		LUM_LOG_INFO("Created shader %s - %s", desc.vertexSource, desc.fragmentSource);
		return mShaders.create_handle(std::move(shader));

	}
	void GLDevice::BindShader ( const ShaderHandle& shader ) {

		LUM_HOTCHK_RETURN_VOID(
			mShaders.exist(shader),
			LUM_SEV_WARN,
			"Shader %d doesn't exist", 
			shader.id
		);

		if (mCurrentShader == shader) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}
		
		mCurrentShader = shader;

		glUseProgram(mShaders[shader].handle);

		LUM_LOG_DEBUG("Shader %d binded", shader.id);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::DeleteShader ( ShaderHandle& shader ) {

		LUM_HOTCHK_RETURN_VOID(
			mShaders.exist(shader),
			LUM_SEV_WARN,
			"Shader %d doesn't exist",
			shader.id
		);

		glDeleteProgram(mShaders[shader].handle);

		mShaders.delete_handle(shader);

		LUM_LOG_DEBUG("Shader %d deleted", shader.id);

	}
	void GLDevice::SetMat4 ( const ShaderHandle& shader, ccharptr location, const math::Mat4& mat ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniformMatrix4fv(loc, 1, GL_FALSE, math::value_ptr(mat));

	}
	void GLDevice::Setf ( const ShaderHandle& shader, ccharptr location, float32 value ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniform1f(loc, value);

	}
	void GLDevice::Seti ( const ShaderHandle& shader, ccharptr location, int32 value ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniform1i(loc, value);

	}
	void GLDevice::SetVec4 (const ShaderHandle& shader, ccharptr location, const math::Vec4& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniform4fv(loc, 1, math::value_ptr(vec));

	}
	void GLDevice::SetVec3 ( const ShaderHandle& shader, ccharptr location, const math::Vec3& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniform3fv(loc, 1, math::value_ptr(vec));

	}
	void GLDevice::SetVec2 ( const ShaderHandle& shader, ccharptr location, const math::Vec2& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTCHK_RETURN_VOID(loc != -1, LUM_SEV_WARN, "Couldn't localize uniform named {}");
		glUniform2fv(loc, 1, math::value_ptr(vec));

	}
	
	bool GLDevice::compile_shader ( GLuint shader ) {

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
	bool GLDevice::link_program ( GLuint program ) {

		glLinkProgram(program);
		int32 success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {

			char buff[2048];
			glGetProgramInfoLog(program, 2048, nullptr, buff);
			LUM_LOG_FATAL("Failed to link program: %s", buff);
			return false;

		}

		return true;
	}

}