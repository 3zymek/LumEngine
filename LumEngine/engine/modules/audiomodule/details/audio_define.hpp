#pragma once
#include "details/e_define.hpp"
#include <iostream>
#include <fmod_errors.h>
namespace FMOD { struct Sound; struct Channel; }
namespace audio {
	namespace detail {

		using EmitterID = uint32_t;
		using AudioID = uint32_t;
		using ChannelID = uint64_t;

		constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
		constexpr unsigned int MAX_CHANNELS_COUNT = 700;

		constexpr EmitterID EMITTER_ID_NULL = 0;
		constexpr AudioID	AUDIO_ID_NULL = 0;
		constexpr ChannelID CHANNEL_ID_NULL = 0;

		struct GenerateAudioID {
			static AudioID Get() {
				return Count()++;
			}
		private:
			static AudioID& Count() {
				static AudioID globalID = AUDIO_ID_NULL + 1;
				return globalID;
			}

		};
		struct GenerateEmitterID {
			static EmitterID Get() {
				return Count()++;
			}
		private:
			static EmitterID& Count() {
				static EmitterID globalID = EMITTER_ID_NULL + 1;
				return globalID;
			}

		};
		struct GenerateChannelID {
			static ChannelID Get() {
				return Count()++;
			}
		private:
			static ChannelID& Count() {
				static ChannelID globalID = CHANNEL_ID_NULL + 1;
				return globalID;
			}
		};

		struct AudioClip {
			FMOD::Sound* sound;
		};

		struct AudioChannel {
			FMOD::Channel* channel = nullptr;
			EmitterID		emitter_id{};
			AudioID			audio_clip{};
		};

		#define FMOD_ASSERT(x) \
			{ FMOD_RESULT r = x; if(r != FMOD_OK) { std::cerr << "[ AUDIO ERROR ] " << FMOD_ErrorString(r) << '\n'; } }

		#ifdef DEBUG_AUDIO
			#define LOG_AUDIO(msg) \
				do { std::cout << "[ AUDIO ] " << msg << '\n'; } while(0);
		#else
			#define LOG_AUDIO(x)
		#endif


	}
}