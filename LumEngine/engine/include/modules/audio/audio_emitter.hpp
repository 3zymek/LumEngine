#pragma once
#include "include/modules/audio/audio_common.hpp"
#include "include/modules/audio/audio_manager.hpp"
#include <unordered_map>
#include <vector>
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