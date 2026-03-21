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