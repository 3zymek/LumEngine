//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: ECS system handling camera and mesh rendering each frame.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum {

	namespace ecs { class MEntityManager; }
	class Window;

	namespace render {

		class Renderer;

		/* @brief ECS system responsible for submitting render objects each frame.
		* Iterates over camera and mesh entities and dispatches draw calls to the Renderer.
		*/
		struct RenderSystem {
		public:

			RenderSystem() = default;

			/* @brief Initializes the system with a renderer instance.
			* @param renderer Valid pointer to an active Renderer.
			*/
			void Initialize( render::Renderer* renderer );

			/* @brief Processes all renderable entities and submits them for drawing.
			* Updates camera matrices from CCamera and CTransform,
			* then submits each CStaticMesh with its material and transform.
			* @param entityMgr Active entity manager to query components from.
			* @param window    Window used to compute the current aspect ratio.
			*/
			void Update( ecs::MEntityManager* entityMgr, Window* window );

		private:

			/* @brief Pointer to the active renderer used for draw call submission. */
			render::Renderer* mRenderer = nullptr;

		};

	} // namespace render

} // namespace lum