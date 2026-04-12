//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Audio module — AHI device and audio manager lifecycle.
//
//=============================================================================//

#include "audio_module.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

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