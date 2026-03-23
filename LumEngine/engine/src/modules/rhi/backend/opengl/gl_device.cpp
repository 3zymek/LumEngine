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

<<<<<<< HEAD
		GLFWwindow* w = static_cast< GLFWwindow* >(window->GetNativeWindow( ));

		glfwMakeContextCurrent( w );

		if (!gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress )) {
			glfwTerminate( );
=======
		GLFWwindow* w = static_cast<GLFWwindow*>(window->GetNativeWindow());

		glfwMakeContextCurrent(w);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
>>>>>>> f8ece7c (Editor in works)
			return;
		}

#		if LUM_ENABLE_DEBUG_RENDER == 1
<<<<<<< HEAD
			glEnable( GL_DEBUG_OUTPUT );
			glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			glDebugMessageCallback( rhi::detail::GLDebugCallback, nullptr );
=======
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(rhi::detail::GLDebugCallback, nullptr);
>>>>>>> f8ece7c (Editor in works)
#		endif

		mWindow = window;

	}

<<<<<<< HEAD
	void GLDevice::Draw( RVertexLayoutHandle layout, uint32 numVertices ) {

		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawArrays( GL_TRIANGLES, 0, numVertices );

		LUM_PROFILER_DRAW_CALL( );
=======
	void GLDevice::Draw( RVertexLayoutHandle layout, uint32 numVertices) {

		LUM_ASSERT(mLayouts.Contains(layout), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[layout].mHandle);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		LUM_PROFILER_DRAW_CALL();
>>>>>>> f8ece7c (Editor in works)

	}

	void GLDevice::DrawInstanced( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) {

	}
	void GLDevice::DrawInstancedBase( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseInstance ) {

	}

	void GLDevice::DrawElements( RVertexLayoutHandle layout, uint32 numIndices ) {

<<<<<<< HEAD
		LUM_ASSERT( mLayouts.Contains( layout ), "Cannot draw, invalid vertex layout" );

		LUM_ASSERT( mBuffers.Contains( mLayouts[ layout ].mElementBuff ), "Layout doesn't have attached any element buffers" );

		glBindVertexArray( mLayouts[ layout ].mHandle );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >(numIndices), GL_UNSIGNED_INT, nullptr );

		LUM_PROFILER_DRAW_CALL( );
=======
		LUM_ASSERT(mLayouts.Contains(layout), "Cannot draw, invalid vertex layout");

		LUM_ASSERT(mBuffers.Contains(mLayouts[layout].mElementBuff), "Layout doesn't have attached any element buffers");

		glBindVertexArray(mLayouts[layout].mHandle);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(numIndices), GL_UNSIGNED_INT, nullptr);

		LUM_PROFILER_DRAW_CALL();
>>>>>>> f8ece7c (Editor in works)

	}

	void GLDevice::DrawElementsInstanced( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances ) {

	}
	void GLDevice::DrawElementsInstancedBase( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseInstance ) {

	}

	void GLDevice::SwapBuffers( ) {

<<<<<<< HEAD
		glfwSwapBuffers( static_cast< GLFWwindow* >(mWindow->GetNativeWindow( )) );

	}

}
=======
		glfwSwapBuffers(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()));
		
	}

} // namespace lum::rhi::gl
>>>>>>> f8ece7c (Editor in works)
