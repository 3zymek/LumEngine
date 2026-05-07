//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Editor viewport handling rendering + camera control.
//
//=============================================================================//
#pragma once

#include "core/editor_common.hpp"
#include "core/editor_camera.hpp"

namespace lum::editor {

	/* @brief Represents the editor viewport panel.
	*
	*  Handles rendering of the scene into a texture,
	*  camera updates, and focus state for input control.
	*/
	class Viewport {
	public:

		/* @brief Updates viewport logic and renders frame.
		*
		*  @param delta Frame time step in seconds.
		*  @param frame Render target texture used for viewport output.
		*  @param dev Rendering device interface.
		*  @param window Native window reference for input/focus handling.
		*  @param renderer Main renderer used to draw scene content.
		*/
		void Update(
			float64 delta,
			rhi::RTextureHandle frame,
			rhi::RenderDevice* dev,
			Window* window,
			render::Renderer* renderer
		);

	private:

		/* @brief Editor camera used for viewport navigation. */
		EditorCamera mCamera;

		/* @brief True if viewport currently has input focus. */
		bool bCameraFocused = true;

	};

} // namespace lum::editor