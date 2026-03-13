#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "ahi/ahi_pch.hpp"

namespace lum::ahi {

	enum class AudioBackend { Fmod };
	
	struct ChannelGroupHandle : cstd::BaseHandle<> { using BaseHandle::BaseHandle; };
	struct SoundHandle : cstd::BaseHandle<> {};
	struct AudioEffectHandle : cstd::BaseHandle<> {};

	inline constexpr ChannelGroupHandle gDefaultGroup(0, 5000);

	struct FPlaybackDescriptor {

		float32 mVolume = 1.0f;
		float32 mPitch = 1.0f;

	};

}