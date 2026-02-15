#pragma once
#include "audio/audio_pch.hpp"
#include "core/core_common.hpp"
namespace FMOD { struct Sound; struct Channel; }
namespace lum {
	namespace audio {

		using EmitterHandle = uint32;
		using AudioHandle	= uint32;
		using ChannelHandle = uint32;

		constexpr uint32 MAX_SOUNDS_COUNT = settings::AUDIO_MAX_SOUNDS_COUNT;
		constexpr uint32 MAX_CHANNELS_COUNT = settings::AUDIO_MAX_CHANNELS_COUNT;

		enum class AudioInitFlags : bitfield {
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
			return static_cast<AudioInitFlags>(static_cast<bitfield>(a) | static_cast<bitfield>(b));
		}
		constexpr AudioInitFlags operator|(AudioInitFlags a, bitfield b) {
			return static_cast<AudioInitFlags>(static_cast<bitfield>(a) | b);
		}
		constexpr AudioInitFlags operator|(bitfield a, AudioInitFlags b) {
			return static_cast<AudioInitFlags>(a | static_cast<bitfield>(b));
		}
		constexpr bitfield operator&(bitfield mask, AudioInitFlags sev) {
			return mask & static_cast<bitfield>(sev);
		}
		constexpr bitfield& operator|=(bitfield& mask, AudioInitFlags sev) {
			mask |= static_cast<bitfield>(sev);
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
				EmitterHandle	emitterID{};
				AudioHandle		audioClip{};
			};

			#define FMOD_ASSERT(x) \
				do { \
					FMOD_RESULT r = x; \
					if(r != FMOD_OK) { /*LUM_LOG_ERROR(FMOD_ErrorString(r)); */ } \
				} while(0)
		}
	}
}