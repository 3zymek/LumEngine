#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "ahi/ahi_pch.hpp"

namespace lum::ahi {

	enum class AudioBackend { Fmod };

    // Flags controlling sound loading, playback behavior and spatialization.
    // Combine with | operator. Maps to backend-specific mode flags via translate_sound_flags().
    enum class SoundFlag : bitfield {

        Default                 = 1 << 0,  // Backend default settings
        CreateStream            = 1 << 1,  // Stream from disk — use for music/long audio
        CreateSample            = 1 << 2,  // Load fully into memory — use for short SFX
        CreateCompressedSample  = 1 << 3,  // Load compressed into memory — saves RAM, slight CPU cost

        NonBlocking = 1 << 4,  // Load asynchronously, check state before playback
        Unique      = 1 << 5,  // Only one instance of this sound can play at a time
        LowMem      = 1 << 6,  // Reduce memory footprint, lower quality

        Spatial3D       = 1 << 10, // Enable 3D spatialization — requires SetSoundPosition()
        Flat2D          = 1 << 11, // No spatialization, ignore 3D position
        LinearRolloff   = 1 << 12, // Volume decreases linearly with distance
        InverseRolloff  = 1 << 13, // Volume decreases naturally (inverse square) with distance

    };
	
	struct ChannelGroupHandle : cstd::BaseHandle<> { using BaseHandle::BaseHandle; };
	struct SoundHandle : cstd::BaseHandle<> {};
	struct AudioEffectHandle : cstd::BaseHandle<> {};

	inline constexpr ChannelGroupHandle gDefaultGroup( 0, 5000 );

	struct FPlaybackDescriptor {

		float32 mVolume = 1.0f;
		float32 mPitch = 1.0f;
        
	};

}
namespace lum { LUM_ENUM_OPERATIONS(ahi::SoundFlag); }