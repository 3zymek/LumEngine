//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Qt offscreen surface OpenGL rendering context implementation.
//
//=============================================================================//
#pragma once

#include "Platform/OpenGLContext.hpp"

class QOffscreenSurface;
class QOpenGLContext;

namespace lum::editor {

	/* @brief OpenGL rendering context backed by Qt QOffscreenSurface. */
	class QtOpenGLContext : public OpenGLContext {
	public:

		void Initialize( ) override;

		/* @brief Presents the rendered frame using Qt. */
		void SwapBuffers( ) override;

		/* @brief Makes the Qt OpenGL context current on the offscreen surface. */
		void MakeCurrent( ) override;

		void* GetProcAddress( const char* functionName ) override;

	private:

		QOffscreenSurface* m_Surface = nullptr; //< Owning pointer to the Qt offscreen surface.
		QOpenGLContext* m_Context = nullptr;   //< Owning pointer to the Qt OpenGL context.

	};

} // namespace lum::editor