//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//

#pragma once

#include "scene/scene_manager.hpp"

namespace lum {

	struct MRenderModule;
	struct MResourceModule;
	struct MAudioModule;
	namespace ev { class EventBus; }

	// Owns the scene manager and wires it up with all engine subsystems.
	// Must be initialized last — depends on platform, resources, render and audio.
	struct MSceneModule {

		MSceneManager mSceneMgr;

		void Initialize( MResourceModule& res, MRenderModule& render, MAudioModule& audio, ev::EventBus& bus );

	};

} // namespace lum