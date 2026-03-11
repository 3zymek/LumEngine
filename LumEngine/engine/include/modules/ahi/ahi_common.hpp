#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "ahi/ahi_pch.hpp"

namespace lum::ahi {

	enum class AudioBackend { Fmod };
	
	struct ChannelHandle : cstd::BaseHandle<> {};
	struct SoundHandle : cstd::BaseHandle<> {};

	struct FSoundInstance {

		SoundHandle mSound;
		float32 mVolume = 1.0f;
		float32 mPitch = 1.0f;
		bool bLoop = false;
		glm::vec3 mPosition;

	};

}