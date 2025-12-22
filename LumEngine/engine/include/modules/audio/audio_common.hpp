#pragma once
#include <iostream>
#include <fmod_errors.h>
#include "include/core/core_common.hpp"
namespace FMOD { struct Sound; struct Channel; }
namespace audio {

	using EmitterHandle = uint32_t;
	using AudioHandle	= uint32_t;
	using ChannelHandle = uint64_t;

	constexpr unsigned int MAX_SOUNDS_COUNT		= settings::MAX_SOUNDS_COUNT;
	constexpr unsigned int MAX_CHANNELS_COUNT	= settings::MAX_CHANNELS_COUNT;

	namespace detail {

		constexpr EmitterHandle EMITTER_ID_NULL = 0;
		constexpr AudioHandle	AUDIO_ID_NULL	= 0;
		constexpr ChannelHandle CHANNEL_ID_NULL = 0;

		struct GenerateAudioID {
			static AudioHandle Get() {
				return Count()++;
			}
		private:
			static AudioHandle& Count() {
				static AudioHandle globalID = AUDIO_ID_NULL + 1;
				return globalID;
			}

		};
		struct GenerateEmitterID {
			static EmitterHandle Get() {
				return Count()++;
			}
		private:
			static EmitterHandle& Count() {
				static EmitterHandle globalID = EMITTER_ID_NULL + 1;
				return globalID;
			}

		};
		struct GenerateChannelID {
			static ChannelHandle Get() {
				return Count()++;
			}
		private:
			static ChannelHandle& Count() {
				static ChannelHandle globalID = CHANNEL_ID_NULL + 1;
				return globalID;
			}
		};


		struct AudioClip {
			FMOD::Sound* sound;
		};

		struct AudioChannel {
			FMOD::Channel*	channel = nullptr;
			EmitterHandle	emitter_id{};
			AudioHandle		audio_clip{};
		};

		#define FMOD_ASSERT(x) \
			do { \
				FMOD_RESULT r = x; \
				if(r != FMOD_OK) { LOG_ERROR(FMOD_ErrorString(r)); } \
			} while(0)


	}
}