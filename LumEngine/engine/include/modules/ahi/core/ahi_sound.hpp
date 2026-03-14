#pragma once

#include "ahi/ahi_common.hpp"

namespace lum::audio { class AudioManager; }

namespace lum::ahi {

	namespace fmod { class FMODDevice; }
	class AudioDevice;

	using InstID = uint32;

	struct FSoundInstance {

		FSoundInstance( SoundHandle sound ) : mSound( sound ), mInstanceID( GenerateID<FSoundInstance, InstID>::Get() ) { }

		void SetVolume(float32 volume) {
			mVolume = volume;
			bDirty = true;
		}

		void SetPitch(float32 pitch) {
			mPitch = pitch;
			bDirty = true;
		}

		void SetLoop(bool loop) {
			bLooped = loop;
			bDirty = true;
		}

		void SetPause(bool pause) { 
			bPaused = pause; 
			bDirty = true;
		}

		void SetDirty(bool dirty) { bDirty = dirty; }
		void SetActive(bool active) { bActive = active; bDirty = true; }
		
		void Stop() {
			bActive = false;
			bDirty = true;
		}

		bool IsPaused		() const noexcept { return bPaused; }
		bool IsActive		() const noexcept { return bActive; }
		bool IsLooped		() const noexcept { return bLooped; }
		bool IsDirty		() const noexcept { return bDirty; }
		float32 GetVolume	() const noexcept { return mVolume; }
		float32 GetPitch	() const noexcept { return mPitch; }
		InstID GetID		() const noexcept { return mInstanceID; }
		SoundHandle GetSound() const noexcept { return mSound; }
		glm::vec3 GetPosition() const noexcept { return mPosition; }

	protected:

		InstID mInstanceID;
		SoundHandle mSound;
		float32 mVolume = 1.0f;
		float32 mPitch = 1.0f;
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };

		bool bPaused = false;
		bool bActive = false;
		bool bLooped = false;
		bool bDirty = true;

	};

	struct FAudioSound {

		uint32 mID = 0;

	};

} // namespace lum::ahi