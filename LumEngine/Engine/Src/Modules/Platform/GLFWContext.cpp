//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: GLFW-based OpenGL rendering context implementation.
//
//=============================================================================//
#include "Platform/GLFWOpenGLContext.hpp"

namespace lum {

	void GLFWOpenGLContext::Initialize( ) {
		// blank
	}

	void GLFWOpenGLContext::SwapBuffers( ) {
		glfwSwapBuffers( &mWindow( ) );
	}

	void GLFWOpenGLContext::MakeCurrent( ) {
		glfwMakeContextCurrent( &mWindow( ) );
	}

	void* GLFWOpenGLContext::GetProcAddress( const char* functionName ) {
		return reinterpret_cast<void*>(glfwGetProcAddress( functionName ));
	}

}