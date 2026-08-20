//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Abstract interface for managing graphics rendering contexts.
//
//=============================================================================//
#pragma once

namespace lum {

	/*
	* @brief Abstract rendering context interface.
	*
	* Provides a platform-independent interface for managing the
	* graphics context and its associated surface.
	*/
	class RenderContext {
	public:

		virtual ~RenderContext( ) = default;

		/* @brief Presents the rendered frame. */
		virtual void SwapBuffers( ) = 0;

		/* @brief Makes this context the current graphics context. */
		virtual void MakeCurrent( ) = 0;

		/*
		* @brief Initializes the rendering context.
		* @return true if initialization succeeded, false otherwise.
		*/
		virtual bool Initialize( ) = 0;
	};

} // namespace lum