#pragma once
#include "audio/audio_common.hpp"
#include "audio/audio_manager.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace audio {
		namespace detail {

			struct AudioInstance {
				AudioHandle id		{	};
				LUMfloat volume		{ 1 };
				LUMfloat pitch		{ 1 };
				LUMbool loop		{ false };
				LUMbool paused		{ false };
			};

			struct AudioEmitter {
				AudioEmitter() { active_clips.reserve(5); }
				ecs::components::TransformComponent* transform = nullptr;
				std::vector<AudioChannel> active_clips;
				std::unordered_map<AudioHandle, AudioInstance> clips;
			};
		}
	}
}