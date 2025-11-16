#pragma once
#include "audiomodule/details/audio_define.hpp"
#include "audiomodule/details/audio_manager.hpp"
#include "core/math/vec3f.hpp"
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
			uint8_t flags = 0;

			static constexpr uint8_t PLAYING	= 0b00000001;
			static constexpr uint8_t PAUSED		= 0b00000010;
			static constexpr uint8_t MUTED		= 0b00000100;
		};

		struct AudioEmitter {
			Vec3f position;
			std::vector<AudioChannel> active_clips;
			std::unordered_map<AudioID, AudioInstance> clips;
		};
	}
}