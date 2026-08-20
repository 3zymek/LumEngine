//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Abstract OpenGL rendering context interface.
//
//=============================================================================//
#pragma once

#include "Platform/RenderContext.hpp"

namespace lum {

	/*
	* @brief Abstract rendering context interface for OpenGL.
	* Extends RenderContext with OpenGL-specific context management.
	*/
	class OpenGLContext : public RenderContext {
	public:

		/* @brief Destroys the OpenGL rendering context. */
		virtual ~OpenGLContext( ) = default;

		/* @brief Makes this OpenGL context the current context. */
		virtual void MakeCurrent( ) = 0;

		/* @brief Presents the rendered OpenGL frame. */
		virtual void SwapBuffers( ) = 0;

		/*
		* @brief Loads OpenGL function using GLAD.
		* @return true if initialization succeeded, false otherwise.
		*/
		virtual bool Initialize( ) = 0;

	};

} // namespace lum