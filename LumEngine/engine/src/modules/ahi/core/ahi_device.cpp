//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Abstract audio device interface (AHI).
//          Defines the backend-agnostic API for sound loading,
//          playback, effects, and 3D audio.
//
//=============================================================================//

#include "ahi/core/ahi_device.hpp"
#include "ahi/backend/fmod_device.hpp"

namespace lum::ahi {

	AudioDevice* CreateDevice( AudioBackend backend ) {
		switch (backend) {
			case AudioBackend::Fmod: return new fmod::FMODDevice();
		}
		return new fmod::FMODDevice( );
	}

}