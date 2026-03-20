//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//
#pragma once

#include "core/utils/logger.hpp"
#include "core/utils/asset_loader.hpp"

#include "event/event_bus.hpp"

#include "platform_module.hpp"
#include "audio_module.hpp"
#include "render_module.hpp"
#include "res_module.hpp"
#include "scene_module.hpp"

#include "imgui.h"

namespace lum {

	class Engine {
	public:

		Engine( ) = default;

		void Initialize( StringView projectDir );
		void SetScene( StringView path );
		void Run( );
		void Finalize( );

	private:

		// Initialization order
		ev::EventBus	mEvBus;
		MAudioModule	mAudio;
		MPlatformModule	mPlatform;
		MResourceModule	mRes;
		MRenderModule	mRender;
		MSceneModule	mScene;

	};

} // namespace lum