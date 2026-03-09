//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL shader operations and management
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RShaderHandle GLDevice::CreateShader( const FShaderDescriptor& desc ) {

		LUM_ASSERT(mShaders.DenseSize() < skMaxShaders, "Max shaders reached");

		FShader shader;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		ccharptr vertexContent = desc.mVertexContent.c_str();
		glShaderSource(vshader, 1, &vertexContent, nullptr);

		ccharptr fragmentContent = desc.mFragmentContent.c_str();
		glShaderSource(fshader, 1, &fragmentContent, nullptr);

		if (!compile_shader(vshader))
			return RShaderHandle{};
		if (!compile_shader(fshader))
			return RShaderHandle{};

		shader.mHandle = glCreateProgram();

		glAttachShader(shader.mHandle, vshader);
		glAttachShader(shader.mHandle, fshader);
		
		glDeleteShader(vshader);
		glDeleteShader(fshader);

		link_program(shader.mHandle);

		RShaderHandle handle = mShaders.Append(std::move(shader));

		LUM_LOG_INFO("Created shader %d", handle.mID);
		return handle;

	}
	void GLDevice::BindShader( RShaderHandle shader ) {

		LUM_HOTCHK_RETURN_VOID(IsValid(shader), LUM_SEV_WARN, "Invalid shader");

		if (mCurrentShader == shader) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}
		
		mCurrentShader = shader;

		glUseProgram(mShaders[shader].mHandle);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::DeleteShader( RShaderHandle& shader ) {

		LUM_HOTCHK_RETURN_VOID(IsValid(shader), LUM_SEV_WARN, "Invalid shader");

		glDeleteProgram(mShaders[shader].mHandle);

		mShaders.Remove(shader);

		LUM_LOG_DEBUG("Shader %d deleted", shader.mID);

	}
	
	bool GLDevice::compile_shader( GLuint shader ) {

		glCompileShader(shader);
		int32 success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			
			char buff[2048];
			glGetShaderInfoLog(shader, 2048, nullptr, buff);
			LUM_LOG_ERROR("Failed to compile shader: %s\n", buff);
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
			LUM_LOG_FATAL("Failed to link program: %s\n", buff);
			return false;

		}

		return true;
	}

}