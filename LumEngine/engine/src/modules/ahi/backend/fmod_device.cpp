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

		return mSounds.Append(std::move(sound));

	}
	void FMODDevice::UnloadSound(SoundHandle& sound) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
		fmodSound->release();
		mSounds.Remove(sound);

	}

	AudioEffectHandle FMODDevice::CreateEffect(const FAudioEffectDescriptor& desc) {
		
		FAudioEffect effect;
		if (desc.mReverb.bEnabled)
			create_reverb_effect(desc.mReverb);

			//case EffectType::LowPass:		{ mSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsp);		break; }
			//case EffectType::HighPass:		{ mSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsp);	break; }
			//case EffectType::Echo:			{ mSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);		break; }
			//case EffectType::Distortion:	{ mSystem->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dsp);	break; }
			//case EffectType::Chorus:		{ mSystem->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dsp);		break; }
			//case EffectType::Flanger:		{ mSystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dsp);		break; }
			//case EffectType::Compressor:	{ mSystem->createDSPByType(FMOD_DSP_TYPE_COMPRESSOR, &dsp);	break; }

		return mEffects.Append(std::move(effect));
		
	}

	

	ChannelGroupHandle FMODDevice::CreateChannelGroup(StringView name) {

		FMOD::ChannelGroup* group;
		mSystem->createChannelGroup(name.data(), &group);

		return mChannelGroups.Append(std::move(group));

	}

	void FMODDevice::PlayOneShot(SoundHandle sound, const FPlaybackDescriptor& desc) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
		FMOD::Channel* channel = nullptr;
		mSystem->playSound(fmodSound, nullptr, false, &channel);
		channel->setVolume(desc.mVolume);
		channel->setPitch(desc.mPitch);

	}
	void FMODDevice::Play(const FSoundInstance& inst, ChannelGroupHandle group) {

		LUM_ASSERT(mSounds.Contains(inst.GetSound()), "Invalid sound");

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[inst.GetSound().mID]);
		FMOD::Channel* channel = nullptr;

		mSystem->playSound(fmodSound, nullptr, inst.IsLooped(), &channel);
		channel->setVolume(inst.GetVolume());
		channel->setPitch(inst.GetPitch());

		if (group != gDefaultGroup) {

			LUM_ASSERT(mChannelGroups.Contains(group), "Invalid group");
			channel->setChannelGroup(static_cast<FMOD::ChannelGroup*>(mChannelGroups[group]));
		
		}

		mChannels.insert({ inst.GetID(), channel });

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

	vptr FMODDevice::create_reverb_effect(const FAudioEffectDescriptor::FReverb& desc) {
		FMOD::DSP* dsp;
		mSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dsp);

		if (desc.mPreset != ReverbPreset::Off) {
			FMOD_REVERB_PROPERTIES props = skReverbPresets[static_cast<byte>(desc.mPreset)];
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, props.DecayTime);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, props.EarlyDelay);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, props.LateDelay);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, props.HFReference);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, props.Diffusion);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, props.WetLevel);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, props.);
			return dsp;
		}

		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, desc.mDecayTime);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, desc.mEarlyDelay);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, desc.mLateDelay);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, desc.mReferenceFreq);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, desc.mDiffusion);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, desc.mWetLevel);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, desc.mDryLevel);
		return dsp;
	}

} // namespace lum::ahi