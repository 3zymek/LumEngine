//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Sound instance and sound asset descriptors for AHI.
//          FSoundInstance carries runtime playback state for a single
//          tracked sound, updated each frame via AudioDevice::Update().
//
//=============================================================================//
#pragma once

#include "ahi/ahi_common.hpp"

namespace lum::ahi {

	using InstID = uint32;

	/* @brief Runtime state for a single tracked sound instance.
	*
	* Owns playback parameters (volume, pitch, position, loop) and a dirty flag
	* that signals AudioDevice::Update() to flush changes to the backend.
	*/
	struct FSoundInstance {

		FSoundInstance( ) : mInstanceID( GenerateID<FSoundInstance, InstID>::Get() ) { }

		InstID		mInstanceID = 0;
		SoundHandle	mSound;
		ChannelGroupHandle mGroup;

		float32		mVolume = 1.0f;
		float32		mPitch = 1.0f;
		glm::vec3	mPosition = { 0.0f, 0.0f, 0.0f };
		float32		mMinDistance = 10.0f;
		float32		mMaxDistance = 50.0f;

		bool bPaused = false;
		bool bPlaying = false; // Defines if instance is in playing state (true even if paused)
		bool bStop = false;
		bool bPlay = false;
		bool bLooped = false;

	};

	/* @brief Lightweight handle to a loaded sound asset. */
	struct FAudioSound {

		uint32 mID = 0;

	};

} // namespace lum::ahi