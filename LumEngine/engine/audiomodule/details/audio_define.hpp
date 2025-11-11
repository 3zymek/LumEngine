#pragma once
#include <vector>
#include <iostream>
#include <fmod.hpp>
#include <unordered_map>
namespace audio {
	namespace detail {

		constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
		constexpr unsigned int MAX_CHANNELS_COUNT = 700;
		
		using EmitterID = uint32_t;
		using AudioID	= uint32_t;
		using ChannelID = uint64_t;

		struct GenerateAudioID {
			static AudioID Get() {
				return Count()++;
			}
		private:
			static AudioID& Count() {
				static AudioID globalID = 1;
				return globalID;
			}

		};
		struct GenerateEmitterID {
			static EmitterID Get() {
				return Count()++;
			}
		private:
			static EmitterID& Count() {
				static EmitterID globalID = 1;
				return globalID;
			}

		};
		struct GenerateChannelID {
			static ChannelID Get() {
				return Count()++;
			}
		private:
			static ChannelID& Count() {
				static ChannelID globalID = 0;
				return globalID;
			}
		};

		struct AudioClip {
			FMOD::Sound* sound;
		};

		struct AudioChannel {
			FMOD::Channel*	channel = nullptr;
			EmitterID		emitter_id{};
			AudioID			audio_clip{};
		};

	}
}