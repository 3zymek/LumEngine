#pragma once
#include "audio/audio_common.hpp"
#include "audio/audio_manager.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace audio {
		namespace detail {

			struct AudioInstance {
				AudioHandle id		{	};
				float32 volume		{ 1 };
				float32 pitch		{ 1 };
				bool loop		{ false };
				bool paused		{ false };
			};

			struct AudioEmitter {
				AudioEmitter() { active_clips.reserve(5); }
				CTransform* transform = nullptr;
				std::vector<AudioChannel> active_clips;
				std::unordered_map<AudioHandle, AudioInstance> clips;
			};
		}
	}
}