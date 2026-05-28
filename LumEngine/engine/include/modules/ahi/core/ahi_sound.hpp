//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Sound instance and sound asset descriptors for AHI.
//          SoundInstance carries runtime playback state for a single
//          tracked sound, updated each frame via AudioDevice::UpdateInstance().
//
//=============================================================================//
#pragma once
#include "ahi/ahi_common.hpp"

namespace lum::ahi {

	using SoundInstanceID = uint32;

	enum class SoundInstanceFlag : uint32 {

		// State
		Paused,
		Playing,
		Looped,

		// Actions
		Stop,
		Play,
		Kill

	};

	/* @brief Runtime state for a single tracked sound instance.
	*
	* Owns playback parameters (volume, pitch, position, loop) and a dirty flag
	* that signals AudioDevice::Update() to flush changes to the backend.
	*/
	struct SoundInstance {

		SoundInstance( ) : mInstanceId( UniqueID<SoundInstance, SoundInstanceID>::Get() ) { }

		SoundInstanceID		mInstanceId = 0;
		SoundHandle			mSound;
		ChannelGroupHandle	mGroup;
		
		float32		mVolume = 1.0f;
		float32		mPitch = 1.0f;
		Vector3		mPosition;
		float32		mMinDistance = 10.0f;
		float32		mMaxDistance = 50.0f;

		Flags<SoundInstanceFlag> mFlags;

	};

	/* @brief Lightweight handle to a loaded sound asset. */
	struct SoundAsset {

		uint32 mId = 0;

	};

} // namespace lum::ahi