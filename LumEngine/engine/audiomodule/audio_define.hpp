#pragma once
#include <vector>
#include <iostream>
#include <fmod.hpp>
#include <unordered_map>
namespace audio {
	namespace detail {

		constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
		constexpr unsigned int MAX_EMITTERS_COUNT = 5000;

		using AudioID = uint64_t;
		using EmitterID = uint64_t;

		struct GenerateAudioID {
			static AudioID Get() {
				static AudioID id = 1;
				return id++;
			}
		private:
			GenerateAudioID() {}
		};
		struct GenerateEmitterID {
			static EmitterID Get() {
				static EmitterID id = 1;
				return id++;
			}
		private:
			GenerateEmitterID() {}
		};
		
		struct SoundAsset {
			SoundAsset(FMOD::Sound* s) : clip(s) {}
			FMOD::Sound* clip = nullptr;
			float	def_volume	= 1.f;
			float	def_pitch	= 1.f;
			bool	def_loop	= false;
		};

		struct AudioInstance {
			FMOD::Channel* channel = nullptr;
			AudioID soundid{};
			float volume = 1.f;
			float pitch = 1.f;
		};

		struct AudioEmitter {
			AudioEmitter() {}

		private:
			std::unordered_map<std::string, AudioID> clips;
			std::vector<AudioInstance> active_sounds;
		};

	}
}