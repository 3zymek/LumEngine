#pragma once
#include "audiomodule/details/audio_define.hpp"
#include "audiomodule/details/audio_manager.hpp"
#include "utils/math/vec3f.hpp"
#include <unordered_map>
#include <vector>
namespace audio {
	namespace detail {

		struct AudioInstance {
			AudioID id{};
			float volume	= 1.f;
			float pitch		= 1.f;
			bool loop		= false;
		};

		struct AudioEmitter {
			Vec3f position;
			std::vector<AudioChannel> active_clips;
			std::unordered_map<AudioID, AudioInstance> clips;
		};
	}
}