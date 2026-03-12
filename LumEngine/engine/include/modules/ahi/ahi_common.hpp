#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "ahi/ahi_pch.hpp"

namespace lum::ahi {

	enum class AudioBackend { Fmod };
	
	struct ChannelGroupHandle : cstd::BaseHandle<> {};
	struct SoundHandle : cstd::BaseHandle<> {};

	struct FPlaybackDescriptor {

		float32 mVolume = 1.0f;
		float32 mPitch = 1.0f;

	};

	enum class AHIError : byte {
		Ok,
		InvalidHandle,
		DeviceNotFound
	};

	inline ccharptr ErrorToString(AHIError err) {
		switch (err) {
		case AHIError::Ok: return "Ok";
		case AHIError::InvalidHandle: return "Invalid handle";
		case AHIError::DeviceNotFound: return "Device not found";
		}

	}

}