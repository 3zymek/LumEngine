//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Audio module — AHI device and audio manager lifecycle.
//
//=============================================================================//

#pragma once

#include "ahi/core/ahi_device.hpp"
#include "audio/audio_manager.hpp"

namespace lum {

	// Owns the AHI audio device and the high-level audio manager.
	// Must be initialized before any scene or gameplay systems that use audio.
	struct MAudioModule {

		ahi::AudioDevice* mAudioDevice = nullptr;
		MAudioManager mAudioMgr;

		void Initialize( ev::EventBus& bus ) {

			mAudioDevice = ahi::CreateDevice( ahi::AudioBackend::Fmod );
			mAudioDevice->Initialize( 512, ahi::InitFlag::RightHanded3D | ahi::InitFlag::DistanceFilter );
			mAudioMgr.Initialize( mAudioDevice );

		}

		void Finalize( ) {

			mAudioDevice->Finalize( );
			delete mAudioDevice;

		}

	};

} // namespace lum