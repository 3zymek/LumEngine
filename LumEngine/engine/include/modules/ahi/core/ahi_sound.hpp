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
	* Use Stop() to deactivate — the backend will release the channel on next Update().
	*/
	struct FSoundInstance {

		FSoundInstance( SoundHandle sound ) : mSound( sound ), mInstanceID( GenerateID<FSoundInstance, InstID>::Get() ) { }

		/* @brief Sets playback volume. Range 0.0 (silent) to 1.0 (full). */
		void SetVolume( float32 volume ) {
			if (volume == mVolume) return;
			mVolume = std::clamp(volume, 0.0f, 1.0f); 
			bDirty = true; 
		}

		/* @brief Sets playback pitch. 1.0 = original, 2.0 = one octave up. */
		void SetPitch( float32 pitch ) {
			if (mPitch == pitch) return;
			mPitch = std::clamp(pitch, 0.0f, 1.0f);
			bDirty = true; 
		}

		/* @brief Sets 3D world-space position of this sound emitter. */
		void SetPosition( glm::vec3 pos ) {
			if (mPosition == pos) return;
			mPosition = pos; 
			bDirty = true; 
		}

		/* @brief Enables or disables looping. */
		void SetLoop( bool loop) {
			if (bLooped == loop) return;
			bLooped = loop;
			bDirty = true;
		}

		/* @brief Pauses or resumes playback without deactivating the instance. */
		void SetPause( bool pause ) { 
			if (bPaused == pause) return;
			bPaused = pause;
			bDirty = true;
		}

		/* @brief Manually clears the dirty flag. Called by AudioDevice::Update() after flush. */
		void SetDirty( bool dirty ) { bDirty = dirty; }

		/* @brief Activates or deactivates the instance. Deactivated instances are stopped on next Update(). */
		void SetActive( bool active ) { 
			if (bActive == active) return;
			bActive = active;
			bDirty = true;
		}

		/* @brief Sets the distance at which the sound plays at full volume. */
		void SetMinDistance( float32 dist ) {
			if (mMinDistance == dist) return;
			mMinDistance = dist;
			bDirty = true;
		}

		/* @brief Sets the distance at which the sound becomes inaudible. */
		void SetMaxDistance( float32 dist ) { 
			if (mMaxDistance == dist) return;
			mMaxDistance = dist;
			bDirty = true;
		}

		/* @brief Deactivates the instance. The backend channel is released on next Update(). */
		void Stop( ) { bActive = false; bDirty = true; }

		bool		IsPaused( ) const noexcept { return bPaused; }
		bool		IsActive( ) const noexcept { return bActive; }
		bool		IsLooped( ) const noexcept { return bLooped; }
		bool		IsDirty( ) const noexcept { return bDirty; }
		float32		GetVolume( ) const noexcept { return mVolume; }
		float32		GetPitch( ) const noexcept { return mPitch; }
		InstID		GetID( ) const noexcept { return mInstanceID; }
		SoundHandle	GetSound( ) const noexcept { return mSound; }
		glm::vec3	GetPosition( ) const noexcept { return mPosition; }
		float32		GetMinDistance( ) const noexcept { return mMinDistance; }
		float32		GetMaxDistance( ) const noexcept { return mMaxDistance; }

	protected:

		InstID		mInstanceID;
		SoundHandle	mSound;
		float32		mVolume = 1.0f;
		float32		mPitch = 1.0f;
		glm::vec3	mPosition = { 0.0f, 0.0f, 0.0f };
		float32		mMinDistance = 10.0f;
		float32		mMaxDistance = 50.0f;

		bool bPaused = false;
		bool bActive = false;
		bool bLooped = false;
		bool bDirty = true;

	};

	/* @brief Lightweight handle to a loaded sound asset. */
	struct FAudioSound {

		uint32 mID = 0;

	};

} // namespace lum::ahi