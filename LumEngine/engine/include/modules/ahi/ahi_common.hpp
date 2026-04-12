//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Common AHI types, handle definitions, and configuration enums.
//          Shared across all AHI headers and backend implementations.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "ahi/ahi_pch.hpp"

namespace lum::ahi {

	/* @brief Selects the audio backend used by AudioDevice. */
	enum class AudioBackend { Fmod };

	/* @brief Flags controlling sound loading, playback behaviour, and spatialization.
	* Combine with | operator. Mapped to backend-specific flags internally.
	*/
	enum class SoundFlag : bitfield {

		Default					= 1 << 0,  // Backend default settings
		CreateStream			= 1 << 1,  // Stream from disk — use for music and long audio
		CreateSample			= 1 << 2,  // Load fully into memory — use for short SFX
		CreateCompressedSample	= 1 << 3,  // Load compressed into memory — saves RAM, slight CPU cost

		NonBlocking = 1 << 4,  // Load asynchronously — check sound state before playback
		Unique		= 1 << 5,  // Only one instance of this sound can play at a time
		LowMem		= 1 << 6,  // Reduce memory footprint at the cost of quality

		Spatial3D		= 1 << 10, // Enable 3D spatialization — requires SetSoundPosition() each frame
		Flat2D			= 1 << 11, // No spatialization — ignore 3D position entirely
		LinearRolloff	= 1 << 12, // Volume decreases linearly with distance
		InverseRolloff	= 1 << 13, // Volume decreases naturally (inverse square) with distance

	};

	struct FListenerAttributes {

		glm::vec3 mPosition = glm::vec3( 0.0f );
		glm::vec3 mVelocity = glm::vec3( 0.0f );
		glm::vec3 mForward = glm::vec3( 0.0f );
		glm::vec3 mUp = glm::vec3( 0.0f );

	};

	/* @brief Flags controlling audio system initialization behaviour.
	* Combine with | operator. Passed once to AudioDevice::Initialize().
	*/
	enum class InitFlag : bitfield {

		Normal				= 1 << 0, // Default settings, no extras
		RightHanded3D		= 1 << 1, // Use right-handed coordinate system — required for OpenGL
		DistanceLowpass		= 1 << 2, // Auto-apply lowpass filter to distant 3D sounds
		DistanceFilter		= 1 << 3, // Full frequency filtering based on distance — more realistic than DistanceLowpass
		ProfilerEnable		= 1 << 4, // Enable live audio profiling — development only
		Vol0BecomesVirtual	= 1 << 5, // Inaudible sounds become virtual instead of consuming CPU — free optimisation

	};

	/* @brief Opaque handle to a channel group. Used for routing and effect processing. */
	struct ChannelGroupHandle : cstd::BaseHandle<> { using BaseHandle::BaseHandle; };

	/* @brief Opaque handle to a loaded sound asset. */
	struct SoundHandle : cstd::BaseHandle<> { };

	/* @brief Opaque handle to a created audio effect chain. */
	struct AudioEffectHandle : cstd::BaseHandle<> { };

	/* @brief Sentinel handle representing the master channel group.
	* Used as the default routing target in AudioDevice::Play().
	*/
	inline constexpr ChannelGroupHandle gDefaultGroup( 0, 5000 );

	/* @brief Parameters for fire-and-forget one-shot playback. */
	struct FPlaybackDescriptor {

		ChannelGroupHandle mGroup = gDefaultGroup;

		float32 mVolume = 1.0f; // Playback volume (0.0 = silent, 1.0 = full)
		float32 mPitch = 1.0f; // Playback pitch (1.0 = original, 2.0 = one octave up)

	};

} // namespace lum::ahi

namespace lum {
	LUM_ENUM_OPERATIONS( ahi::SoundFlag );
	LUM_ENUM_OPERATIONS( ahi::InitFlag );
} // namespace lum