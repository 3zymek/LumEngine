//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL device state core management and rendering commands
//          Viewport, color/depth/stencil clearing, draw calls,
//          frame begin/end, and state queries
//
//=============================================================================//

#include "rhi/backend/gl_device.hpp"
#include "platform/window.hpp"

namespace lum::rhi::gl {

	void GLDevice::Initialize( Window* window ) {

		GLFWwindow* w = static_cast< GLFWwindow* >(window->GetNativeWindow( ));

		glfwMakeContextCurrent( w );

		if (!gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress )) {
			glfwTerminate( );
			return;
		}

#		if LUM_ENABLE_DEBUG_RENDER == 1
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( rhi::detail::GLDebugCallback, nullptr );
#		endif

		mWindow = window;

		glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

	}

	void GLDevice::Draw( RVertexLayoutHandle layout, uint32 numVertices ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawArrays( GL_TRIANGLES, 0, numVertices );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawInstanced( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawArraysInstanced( GL_TRIANGLES, 0, numVertices, numInstances );

		LUM_PROFILER_DRAW_CALL( );

	}
	void GLDevice::DrawInstancedBase( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseInstance ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawArraysInstancedBaseInstance( GL_TRIANGLES, 0, numVertices, numInstances, baseInstance );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawElements( RVertexLayoutHandle layout, uint32 numIndices ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( mBuffers.Contains( mLayouts[ layout ].mElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >(numIndices), GL_UNSIGNED_INT, nullptr );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::DrawElementsInstanced( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( mBuffers.Contains( mLayouts[ layout ].mElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawElementsInstanced( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr, numInstances );

		LUM_PROFILER_DRAW_CALL( );

	}
	void GLDevice::DrawElementsInstancedBase( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseInstance ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );
		LUM_ASSERT( mBuffers.Contains( mLayouts[ layout ].mElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawElementsInstancedBaseInstance( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr, numIndices, baseInstance );

		LUM_PROFILER_DRAW_CALL( );

	}

	void GLDevice::SwapBuffers( ) {

		glfwSwapBuffers( static_cast< GLFWwindow* >(mWindow->GetNativeWindow( )) );

	}

} // namespace lum::rhi::gl