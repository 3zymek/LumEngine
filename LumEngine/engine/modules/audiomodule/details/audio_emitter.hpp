#pragma once
#include "audiomodule/details/audio_define.hpp"
#include "audiomodule/details/audio_manager.hpp"
#include <unordered_map>
#include <vector>
namespace audio {
	namespace detail {

		struct AudioInstance {
			AudioID id		{};
			float volume	{ 1 };
			float pitch		{ 1 };
			bool loop		{ false };
			bool paused		{ false };
		};

		struct AudioEmitter {
			std::vector<AudioChannel> active_clips;
			std::unordered_map<AudioID, AudioInstance> clips;
		};
	}
}