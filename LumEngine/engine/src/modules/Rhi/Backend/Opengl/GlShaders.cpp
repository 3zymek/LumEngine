//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL shader operations and management
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	ShaderHandle GLDevice::CreateShader( const ShaderCreateInfo& desc ) {

		LUM_ASSERT( m_Shaders.DenseSize( ) < skMaxShaders, "Max shaders reached" );

		Shader shader;

		GLuint vshader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER );

		const char* vertexContent = desc.m_VertexContent.c_str( );
		glShaderSource( vshader, 1, &vertexContent, nullptr );

		const char* fragmentContent = desc.m_FragmentContent.c_str( );
		glShaderSource( fshader, 1, &fragmentContent, nullptr );

		if (!compile_shader( vshader ))
			return ShaderHandle{};
		if (!compile_shader( fshader ))
			return ShaderHandle{};

		shader.m_Handle = glCreateProgram( );

		glAttachShader( shader.m_Handle, vshader );
		glAttachShader( shader.m_Handle, fshader );

		glDeleteShader( vshader );
		glDeleteShader( fshader );

		link_program( shader.m_Handle );

		return m_Shaders.Append( std::move( shader ) );

	}
	void GLDevice::BindShader( ShaderHandle shader ) {

		if (!IsValid( shader )) return;

		if (m_CurrentShader == shader) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		m_CurrentShader = shader;

		glUseProgram( m_Shaders[ shader ].m_Handle );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::Delete( ShaderHandle& shader ) {

		LUM_RETURN_IF( !IsValid( shader ), LUM_SEV_WARN, "Invalid shader" );

		glDeleteProgram( m_Shaders[ shader ].m_Handle );

		m_Shaders.Remove( shader );

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

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1f( m_Shaders[ shader ].m_Handle, location, value );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, uint32 value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1ui( m_Shaders[ shader ].m_Handle, location, value );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, bool value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform1i( m_Shaders[ shader ].m_Handle, location, value ? 1 : 0 );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector2& value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform2f( m_Shaders[ shader ].m_Handle, location, value.m_X, value.m_Y );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector3& value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform3f( m_Shaders[ shader ].m_Handle, location, value.m_X, value.m_Y, value.m_Z );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Vector4& value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform4f( m_Shaders[ shader ].m_Handle, location, value.m_X, value.m_Y, value.m_Z, value.m_W );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Matrix3& value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniformMatrix3fv( m_Shaders[ shader ].m_Handle, location, 1, GL_FALSE, value.Data( ) );

	}
	void GLDevice::SetUniform( ShaderHandle shader, uint32 location, const Matrix4& value ) {

		LUM_ASSERT( m_Shaders.Contains( shader ), "Invalid shader" );
		glProgramUniform4fv( m_Shaders[ shader ].m_Handle, location, 1, value.Data( ) );

	}

}