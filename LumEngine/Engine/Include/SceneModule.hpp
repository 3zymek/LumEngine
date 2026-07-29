//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//
#pragma once

#include "Scene/SceneManager.hpp"
#include "Entity/TransformSystem.hpp"

namespace lum {

	class RenderModule;
	class ResourceModule;
	class AudioModule;
	namespace ev { class EventBus; }

	// Owns the scene manager and wires it up with all engine subsystems.
	// Must be initialized last — depends on platform, resources, render and audio.
	class SceneModule {
	public:

		SceneManager mSceneMgr;

		void Initialize( ResourceModule& res, RenderModule& render, AudioModule& audio, ev::EventBus& bus );

	};

} // namespace lum