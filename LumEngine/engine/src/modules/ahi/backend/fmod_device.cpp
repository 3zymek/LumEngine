#include "ahi/backend/fmod_device.hpp"

namespace lum::ahi::fmod {


	void FMODDevice::Initialize() {

		FMOD::System_Create(&mSystem);
		FMOD_RESULT result = mSystem->init(512, FMOD_INIT_NORMAL, nullptr);
		LUM_ASSERT(result == FMOD_OK, "FMOD init failed");

	}
	void FMODDevice::Finalize() {

		mSystem->release();

	}

	SoundHandle FMODDevice::LoadSound(StringView path) {

		FMOD::Sound* sound = nullptr;
		mSystem->createSound(path.data(), FMOD_DEFAULT, nullptr, &sound);

		return mSounds.Append(sound);

	}
	void FMODDevice::UnloadSound(SoundHandle& sound) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
		fmodSound->release();
		mSounds.Remove(sound);

	}

	ChannelGroupHandle FMODDevice::CreateChannelGroup(StringView name) {

		FMOD::ChannelGroup* group;
		mSystem->createChannelGroup(name.data(), &group);

		ChannelGroupHandle handle = mChannelGroups.Append(group);

		return handle;
	}

	void FMODDevice::PlayOneShot(SoundHandle sound, const FPlaybackDescriptor& desc) {

		FMOD::DSP* reverb;
		mSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &reverb);

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
		FMOD::Channel* channel = nullptr;
		mSystem->playSound(fmodSound, nullptr, false, &channel);
		channel->setVolume(desc.mVolume);
		channel->setPitch(desc.mPitch);
		channel->addDSP(0, reverb);


	}
	void FMODDevice::Play(const FSoundInstance& inst) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[inst.GetSound().mID]);
		FMOD::Channel* channel = nullptr;
		mSystem->playSound(fmodSound, nullptr, inst.IsLooped(), &channel);

		channel->setVolume(inst.GetVolume());
		channel->setPitch(inst.GetVolume());

		mChannels.insert({ inst.GetID(), channel});

	}

	void FMODDevice::Update(std::vector<FSoundInstance>& instances) {

		for (auto& instance : instances) {

			FMOD::Channel* channel = static_cast<FMOD::Channel*>(mChannels[instance.GetID()]);
			bool playing;
			channel->isPlaying(&playing);

			if (!playing) { mChannels.erase(instance.GetID()); continue; }

			if (!instance.IsDirty()) continue;

			channel->setVolume(instance.GetVolume());
			channel->setPitch(instance.GetPitch());

			instance.SetDirty(false);

		}


		mSystem->update();

	}

} // namespace lum::ahi