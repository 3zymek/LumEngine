#pragma once
#include "audio/audio_pch.hpp"
#include "core/core_common.hpp"
namespace FMOD { struct Sound; struct Channel; }
namespace lum {
	namespace audio {

		using EmitterHandle = uint32_t;
		using AudioHandle	= uint32_t;
		using ChannelHandle = uint64_t;

		constexpr unsigned int MAX_SOUNDS_COUNT = settings::AUDIO_MAX_SOUNDS_COUNT;
		constexpr unsigned int MAX_CHANNELS_COUNT = settings::AUDIO_MAX_CHANNELS_COUNT;

		enum class AudioInitFlags : Bit {
			Normal				= 0x00000000, // Default audio initialization (no extra features)
			RightHanded3D		= 0x00000004, // Use right-handed coordinate system for 3D audio (matches OpenGL / RH engines)
			ChannelLowpass		= 0x00000100, // Enable per-channel low-pass filter support (muffling, occlusion effects)
			DistanceFilter		= 0x00000200, // Enable distance-based audio filtering (automatic DSP based on source distance)
			Vol0BecomesVirtual	= 0x00020000, // Voices with volume = 0 become virtual (not mixed, better performance)
			GeometryClosest		= 0x00040000, // Audio geometry occlusion uses closest surface only (faster, less accurate)
			MemoryTracking		= 0x00400000, // Enable audio memory allocation tracking (debug/profiling only)
			Default				= RightHanded3D | Vol0BecomesVirtual // Prefered config
		};

		constexpr AudioInitFlags operator|(AudioInitFlags a, AudioInitFlags b) {
			return static_cast<AudioInitFlags>(static_cast<Bit>(a) | static_cast<Bit>(b));
		}
		constexpr AudioInitFlags operator|(AudioInitFlags a, Bit b) {
			return static_cast<AudioInitFlags>(static_cast<Bit>(a) | b);
		}
		constexpr AudioInitFlags operator|(Bit a, AudioInitFlags b) {
			return static_cast<AudioInitFlags>(a | static_cast<Bit>(b));
		}
		constexpr Bit operator&(Bit mask, AudioInitFlags sev) {
			return mask & static_cast<Bit>(sev);
		}
		constexpr Bit& operator|=(Bit& mask, AudioInitFlags sev) {
			mask |= static_cast<Bit>(sev);
			return mask;
		}

		namespace detail {

			constexpr EmitterHandle EMITTER_ID_NULL = 0;
			constexpr AudioHandle	AUDIO_ID_NULL = 0;
			constexpr ChannelHandle CHANNEL_ID_NULL = 0;


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
}