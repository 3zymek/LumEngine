//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Audio module — AHI device and audio manager lifecycle.
//
//=============================================================================//
#pragma once
#include "Ahi/Core/AhiDevice.hpp"
#include "Audio/AudioManager.hpp"

namespace lum {

	// Owns the AHI audio device and the high-level audio manager.
	// Must be initialized before any scene or gameplay systems that use audio.
	class MAudioModule {
	public:

		ahi::AudioDevice* mAudioDevice = nullptr;
		AudioManager mAudioMgr;

		void Initialize( ev::EventBus& bus );
		void Finalize( );

	};

} // namespace lum