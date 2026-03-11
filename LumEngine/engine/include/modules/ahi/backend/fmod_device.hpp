#pragma once

#include "ahi/core/ahi_device.hpp"

namespace lum::ahi {

	class FMODDevice : public AudioDevice {
	public:

		void Initialize() override {

			FMOD::System_Create(&mSystem);
			FMOD_RESULT result = mSystem->init(512, FMOD_INIT_NORMAL, nullptr);
			LUM_ASSERT(result == FMOD_OK, "FMOD init failed");

		}
		void Finalize() override {

			mSystem->release();

		}

		SoundHandle LoadSound(StringView path) override {

			FMOD::Sound* sound = nullptr;
			mSystem->createSound(path.data(), FMOD_DEFAULT, nullptr, &sound);

			auto handle = mSounds.Append(sound);

			return handle;

		}
		void UnloadSound(SoundHandle& sound) override {
			
			if (!mSounds.Contains(sound)) {
				LUM_LOG_WARN("Invalid sound handle");
				return;
			}

			FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
			fmodSound->release();
			mSounds.Remove(sound);


		}

		virtual void Play(SoundHandle sound) override {

			if (!mSounds.Contains(sound)) {
				LUM_LOG_WARN("Invalid sound handle");
				return;
			}

			FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
			FMOD::Channel* channel = nullptr;
			mSystem->playSound(fmodSound, nullptr, false, &channel);


		}
		virtual void Play(FSoundInstance sound) override {

			if (!mSounds.Contains(sound.mSound)) {
				LUM_LOG_WARN("Invalid sound handle");
				return;
			}




		}


	private:

		FMOD::System* mSystem = nullptr;
	

		

	};

}