//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Audio module — AHI device and audio manager lifecycle.
//
//=============================================================================//

#include "Ahi/AhiTypes.hpp"
#include "Ahi/Core/AhiDevice.hpp"
#include "AudioModule.hpp"
#include "Event/EventBus.hpp"

namespace lum {

	//=======================================================//
	// Public
	//=======================================================//

	void AudioModule::Initialize( ev::EventBus& bus ) {

		m_AudioDevice = ahi::CreateDevice( ahi::AudioBackend::Fmod );
		m_AudioDevice( ).Initialize( 512, ahi::InitFlag::RightHanded3D | ahi::InitFlag::DistanceFilter );
		m_AudioMgr.Initialize( m_AudioDevice.Ref( ), bus );

	}

	void AudioModule::Finalize( ) {

		m_AudioDevice( ).Finalize( );

	}

} // namespace lum