//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL device state core management and rendering commands
//          Viewport, color/depth/stencil clearing, draw calls,
//          frame begin/end, and state queries
//
//=============================================================================//

#include "Rhi/Backend/GlDevice.hpp"
#include "Platform/Window.hpp"
#include "Platform/OpenGLContext.hpp"

namespace lum::rhi::gl {

	void GLDevice::Initialize( IRenderContext& ctx ) {

		auto* glContext = static_cast<OpenGLContext*>(&ctx);

		glContext->MakeCurrent( );

		static SafePtr<OpenGLContext> sContext = nullptr;
		sContext = glContext;

		bool result = gladLoadGLLoader(
			reinterpret_cast<GLADloadproc>(
				+[]( const char* functionName ) -> void* {
					return sContext( ).GetProcAddress( functionName );
				}
			)
		);

		if (!result)
			return;

#		if LUM_ENABLE_DEBUG_RENDER == 1

			glEnable( GL_DEBUG_OUTPUT );
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			glDebugMessageCallback( rhi::detail::GLDebugCallback, nullptr );

#		endif

		m_RenderContext = glContext;

		glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

	}

	void GLDevice::Draw( VertexLayoutHandle layout, uint32 numVertices ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawArrays( GL_TRIANGLES, 0, numVertices );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawInstanced( VertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawArraysInstanced( GL_TRIANGLES, 0, numVertices, numInstances );

		LUM_PROFILER_DRAW_CALL( );

	}
	void GLDevice::DrawInstancedBase( VertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseInstance ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawArraysInstancedBaseInstance( GL_TRIANGLES, 0, numVertices, numInstances, baseInstance );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawElements( VertexLayoutHandle layout, uint32 numIndices ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( m_Buffers.Contains( m_Layouts[ layout ].m_ElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(numIndices), GL_UNSIGNED_INT, nullptr );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawElementsInstanced( VertexLayoutHandle layout, uint32 numIndices, uint32 numInstances ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( m_Buffers.Contains( m_Layouts[ layout ].m_ElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawElementsInstanced( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr, numInstances );

		LUM_PROFILER_DRAW_CALL( );

	}
	void GLDevice::DrawElementsInstancedBase( VertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseInstance ) {

		LUM_ASSERT( m_Layouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( m_Buffers.Contains( m_Layouts[ layout ].m_ElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( m_Layouts[ layout ].m_Handle );
		glDrawElementsInstancedBaseInstance( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr, numIndices, baseInstance );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::SwapBuffers( ) {

		m_RenderContext->SwapBuffers( );

	}

} // namespace lum::rhi::gl