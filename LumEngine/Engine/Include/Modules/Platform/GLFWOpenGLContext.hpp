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
		GLFWOpenGLContext( GLFWwindow* window ) : mWindow( window ) {}

		/*
		* @brief Makes the GLFW OpenGL context current.
		*/
		void MakeCurrent( ) override;

		/*
		* @brief Presents the rendered frame using GLFW.
		*/
		void SwapBuffers( ) override;

		/*
		* @brief Initializes the GLFW OpenGL context.
		* @return true if initialization succeeded, false otherwise.
		*/
		bool Initialize( ) override;

	private:

		SafePtr<GLFWwindow> mWindow = nullptr; //< Non-owning pointer to the GLFW window.

	};

} // namespace lum