//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//

#pragma once

#include "render/renderer.hpp"
#include "render/render_system.hpp"

namespace lum {

	struct MPlatformModule;
	struct MResourceModule;
	namespace ev { class EventBus; }

	// Owns the renderer and render system.
	// Must be initialized after MPlatformModule and MResourceModule.
	struct MRenderModule {

		render::Renderer	 mRenderer;
		render::RenderSystem mRenderSys;

		void Initialize( MPlatformModule& platform, MResourceModule& res, ev::EventBus& bus );

	};

} // namespace lum