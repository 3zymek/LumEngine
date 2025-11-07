#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fmod.hpp>
namespace audio {
	namespace detail {

		constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
		constexpr unsigned int MAX_EMITTERS_COUNT = 5000;

		using AudioID = uint64_t;
		using EmitterID = uint64_t;

		struct GenerateAudioID {
			static AudioID Get() {
				static AudioID id = 0;
				return id++;
			}
		private:
			GenerateAudioID() {}
		};

		struct GenerateEmitterID {
			static EmitterID Get() {
				static EmitterID id = 0;
				return id++;
			}
		private:
			GenerateEmitterID() {}
		};
		
		struct AudioInstance {
			FMOD::Channel* channel = nullptr;
			AudioID audioID{};
			float volume = 1.f;
			float pitch = 1.f;
		};
		struct AudioEmitter {
			AudioEmitter() {}
			std::unordered_map<std::string, AudioID> clips;
			std::vector<AudioInstance> active_sounds;
		};

	}
}