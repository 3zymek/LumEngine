//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Audio module — AHI device and audio manager lifecycle.
//
//=============================================================================//

#pragma once

#include "audio_module.hpp"

namespace lum {

	void MAudioModule::Initialize( ev::EventBus& bus ) {

		mAudioDevice = ahi::CreateDevice( ahi::AudioBackend::Fmod );
		mAudioDevice->Initialize( 512, ahi::InitFlag::RightHanded3D | ahi::InitFlag::DistanceFilter );
		mAudioMgr.Initialize( mAudioDevice, &bus );

	}

	void MAudioModule::Finalize( ) {

		mAudioDevice->Finalize( );
		delete mAudioDevice;

	}

} // namespace lum