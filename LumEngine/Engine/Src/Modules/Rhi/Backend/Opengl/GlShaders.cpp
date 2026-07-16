//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL shader operations and management
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	ShaderHandle GLDevice::CreateShader( const ShaderCreateInfo& desc ) {

		LUM_ASSERT( mShaders.DenseSize( ) < skMaxShaders, "Max shaders reached" );

		Shader shader;

		GLuint vshader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER );

		ccharptr vertexContent = desc.mVertexContent.c_str( );
		glShaderSource( vshader, 1, &vertexContent, nullptr );

		ccharptr fragmentContent = desc.mFragmentContent.c_str( );
		glShaderSource( fshader, 1, &fragmentContent, nullptr );

		if (!compile_shader( vshader ))
			return ShaderHandle{};
		if (!compile_shader( fshader ))
			return ShaderHandle{};

		shader.mHandle = glCreateProgram( );

		glAttachShader( shader.mHandle, vshader );
		glAttachShader( shader.mHandle, fshader );

		glDeleteShader( vshader );
		glDeleteShader( fshader );

		link_program( shader.mHandle );

		return mShaders.Append( std::move( shader ) );

	}
	void GLDevice::BindShader( ShaderHandle shader ) {

		if (!IsValid( shader )) return;

		if (mCurrentShader == shader) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		mCurrentShader = shader;

		glUseProgram( mShaders[ shader ].mHandle );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::Delete( ShaderHandle& shader ) {

		LUM_RETURN_IF( !IsValid( shader ), LUM_SEV_WARN, "Invalid shader" );

		glDeleteProgram( mShaders[ shader ].mHandle );

		mShaders.Remove( shader );

	}

	bool GLDevice::compile_shader( GLuint shader ) {

		glCompileShader( shader );
		int32 success;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
		if (!success) {

			char buff[ 2048 ];
			glGetShaderInfoLog( shader, 2048, nullptr, buff );
			LUM_LOG_ERROR( "Failed to compile shader: %s\n", buff );
			return false;

		}

		return true;
	}
	bool GLDevice::link_program( GLuint program ) {

		glLinkProgram( program );
		int32 success;
		glGetProgramiv( program, GL_LINK_STATUS, &success );
		if (!success) {

			char buff[ 2048 ];
			glGetProgramInfoLog( program, 2048, nullptr, buff );
			LUM_LOG_FATAL( "Failed to link program: %s\n", buff );
			return false;

		}

		return true;
	}

	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, float32 value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1f( mShaders[ shader ].mHandle, location, value );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, uint32 value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1ui( mShaders[ shader ].mHandle, location, value );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, bool value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1i( mShaders[ shader ].mHandle, location, value ? 1 : 0 );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector2& value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform2f( mShaders[ shader ].mHandle, location, value.mX, value.mY );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector3& value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform3f( mShaders[ shader ].mHandle, location, value.mX, value.mY, value.mZ );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector4& value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform4f( mShaders[ shader ].mHandle, location, value.mX, value.mY, value.mZ, value.mW );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Matrix3& value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniformMatrix3fv( mShaders[ shader ].mHandle, location, 1, GL_FALSE, value.Data( ) );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Matrix4& value ) {

		LUM_ASSERT( mShaders.Contains( shader ), "Invalid shader" );
		glProgramUniform4fv( mShaders[ shader ].mHandle, location, 1, value.Data( ) );

	}

}