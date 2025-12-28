#pragma once
#include "audio/audio_common.hpp"
#include "audio/audio_manager.hpp"
#include "core/core_pch.hpp"
namespace audio {
	namespace detail {

		struct AudioInstance {
			AudioHandle id		{};
			float volume	{ 1 };
			float pitch		{ 1 };
			bool loop		{ false };
			bool paused		{ false };
		};

		struct AudioEmitter {
			std::vector<AudioChannel> active_clips;
			std::unordered_map<AudioHandle, AudioInstance> clips;
		};
	}
}