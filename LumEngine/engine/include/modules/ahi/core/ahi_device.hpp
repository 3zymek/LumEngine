#pragma once

#include "ahi/ahi_common.hpp"
#include "core/limits.hpp"
#include "ahi/core/ahi_clip.hpp"

namespace lum::ahi {

	class AudioDevice {
	public:

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

		virtual SoundHandle LoadSound(StringView path) = 0;
		virtual void UnloadSound(SoundHandle& sound) = 0;

		virtual void Play(SoundHandle sound) = 0;
		virtual void Play(FSoundInstance sound) = 0;
		virtual void Stop(SoundHandle sound) = 0;
		virtual void Pause(SoundHandle sound) = 0;
		virtual void Resume(SoundHandle sound) = 0;

		virtual void SetVolume(SoundHandle sound, float32 value) = 0;
		virtual void SetPitch(SoundHandle sound, float32 value) = 0;
		virtual void SetLooping(SoundHandle sound, bool loop) = 0;

		virtual void SetPosition(SoundHandle sound, glm::vec3 pos) = 0;
		virtual void SetListenerPosition() = 0;

		virtual void Update() = 0;
		
	protected:

		cstd::HandlePool<SoundHandle, vptr> mSounds{ limits::gMaxAudioSounds };


	};

}