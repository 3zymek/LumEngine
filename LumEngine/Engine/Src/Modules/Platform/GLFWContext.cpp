//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: GLFW-based OpenGL rendering context implementation.
//
//=============================================================================//
#include "Platform/GLFWOpenGLContext.hpp"

namespace lum {

	void GLFWOpenGLContext::SwapBuffers( ) {
		glfwSwapBuffers( &mWindow( ) );
	}

	void GLFWOpenGLContext::MakeCurrent( ) {
		glfwMakeContextCurrent( &mWindow( ) );
	}

	bool GLFWOpenGLContext::Initialize( ) {
		return gladLoadGLLoader(
				reinterpret_cast<GLADloadproc>(glfwGetProcAddress)
		);
	}

}