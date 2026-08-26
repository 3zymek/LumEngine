//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: GLFW-based OpenGL rendering context implementation.
//
//=============================================================================//
#pragma once

#include "Core/Utils/SafePtr.hpp"
#include "Platform/OpenGLContext.hpp"

class GLFWwindow;

namespace lum {

	/*
	* @brief OpenGL rendering context backed by a GLFW window.
	*/
	class GLFWOpenGLContext : public OpenGLContext {
	public:

		/*
		* @brief Constructs a GLFW rendering context.
		* @param window GLFW window used by the rendering context.
		*/
		GLFWOpenGLContext( GLFWwindow* window ) : m_Window( window ) {}

		void Initialize( ) override;

		/* @brief Makes the GLFW OpenGL context current. */
		void MakeCurrent( ) override;

		/* @brief Presents the rendered frame using GLFW. */
		void SwapBuffers( ) override;

		void* GetProcAddress( const char* functionName ) override;

	private:

		SafePtr<GLFWwindow> m_Window = nullptr; //< Non-owning pointer to the GLFW window.

	};

} // namespace lum