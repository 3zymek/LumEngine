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

	SoundHandle FMODDevice::LoadSound(StringView path, Flags<SoundFlag> flags) {
		
		FMOD::Sound* sound = nullptr;

		FMOD_MODE fmodFlags = translate_sound_flags(flags);

		mSystem->createSound(path.data(), fmodFlags, nullptr, &sound);

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
			effect.mDSPs.push_back(create_reverb_effect(desc.mReverb));
		if (desc.mFreqPass.mLow.bEnabled)
			effect.mDSPs.push_back(create_frequency_effect(desc.mFreqPass.mLow, detail::FrequnecyType::Low));
		if (desc.mFreqPass.mHigh.bEnabled)
			effect.mDSPs.push_back(create_frequency_effect(desc.mFreqPass.mHigh, detail::FrequnecyType::High));
		if (desc.mEcho.bEnabled)
			effect.mDSPs.push_back(create_echo_effect(desc.mEcho));
		if (desc.mDistortion.bEnabled)
			effect.mDSPs.push_back(create_distortion_effect(desc.mDistortion));
		if (desc.mChorus.bEnabled)
			effect.mDSPs.push_back(create_chorus_effect(desc.mChorus));
		if (desc.mFlange.bEnabled)
			effect.mDSPs.push_back(create_flange_effect(desc.mFlange));
		if (desc.mCompressor.bEnabled)
			effect.mDSPs.push_back(create_compressor_effect(desc.mCompressor));

		return mEffects.Append(std::move(effect));
		
	}

	void FMODDevice::AddEffectToGroup( ChannelGroupHandle group, AudioEffectHandle effect ) {

		

		FMOD::ChannelGroup* fmodGroup = static_cast<FMOD::ChannelGroup*>(mChannelGroups[group]);
		FAudioEffect sfx = mEffects[effect];

		for (int32 i = 0; i < sfx.mDSPs.size(); i++) {

			fmodGroup->addDSP(i, static_cast<FMOD::DSP*>(sfx.mDSPs[i]));

		}

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
	void FMODDevice::Play(FSoundInstance& inst, ChannelGroupHandle group) {

		LUM_ASSERT(mSounds.Contains(inst.GetSound()), "Invalid sound");

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[inst.GetSound().mID]);
		FMOD::Channel* channel = nullptr;

		mSystem->playSound(fmodSound, nullptr, inst.IsPaused(), &channel);
		channel->setVolume(inst.GetVolume());
		channel->setPitch(inst.GetPitch());

		if (group != gDefaultGroup) {

			LUM_ASSERT(mChannelGroups.Contains(group), "Invalid group");
			channel->setChannelGroup(static_cast<FMOD::ChannelGroup*>(mChannelGroups[group]));
		
		}

		inst.SetActive(true);
		mChannels.insert({ inst.GetID(), channel });

	}

	void FMODDevice::StopAll() {

	}

	void FMODDevice::SetMasterVolume(float32) {

	}
	void FMODDevice::AddEffectToSound(SoundHandle, AudioEffectHandle) {

	}
	void FMODDevice::Set3DListenerPosition(glm::vec3, glm::vec3 forward, glm::vec3 up) {

	}

	void FMODDevice::Update(std::vector<FSoundInstance>& instances) {

		for (auto& instance : instances) {

			auto it = mChannels.find(instance.GetID());
			if (it == mChannels.end()) continue;

			FMOD::Channel* channel = static_cast<FMOD::Channel*>(it->second);
			bool playing;
			channel->isPlaying(&playing);

			if (!playing) { mChannels.erase(instance.GetID()); continue; }

			if (!instance.IsActive()) {
				channel->stop();
				mChannels.erase(instance.GetID());
				continue;
			}

			channel->setPaused(instance.IsPaused());

			if (!instance.IsDirty()) continue;

			channel->setVolume(instance.GetVolume());
			channel->setPitch(instance.GetPitch());

			if (instance.IsLooped())
				channel->setMode(FMOD_LOOP_NORMAL);
			else
				channel->setMode(FMOD_LOOP_OFF);

			instance.SetDirty(false);

		}


		mSystem->update();

	}

	FMOD_MODE FMODDevice::translate_sound_flags(Flags<SoundFlag> flags) {

		FMOD_MODE mode = 0;

		if (flags.Has(SoundFlag::Default))                mode |= FMOD_DEFAULT;
		if (flags.Has(SoundFlag::CreateStream))           mode |= FMOD_CREATESTREAM;
		if (flags.Has(SoundFlag::CreateSample))           mode |= FMOD_CREATESAMPLE;
		if (flags.Has(SoundFlag::CreateCompressedSample)) mode |= FMOD_CREATECOMPRESSEDSAMPLE;
		if (flags.Has(SoundFlag::NonBlocking))            mode |= FMOD_NONBLOCKING;
		if (flags.Has(SoundFlag::Unique))                 mode |= FMOD_UNIQUE;
		if (flags.Has(SoundFlag::LowMem))                 mode |= FMOD_LOWMEM;
		if (flags.Has(SoundFlag::Spatial3D))              mode |= FMOD_3D;
		if (flags.Has(SoundFlag::Flat2D))                 mode |= FMOD_2D;
		if (flags.Has(SoundFlag::LinearRolloff))          mode |= FMOD_3D_LINEARROLLOFF;
		if (flags.Has(SoundFlag::InverseRolloff))         mode |= FMOD_3D_INVERSEROLLOFF;

		return mode;

	}

	FMOD::DSP* FMODDevice::create_reverb_effect(const FAudioEffectDescriptor::FReverb& desc) {

		FMOD::DSP* dsp = nullptr;
		mSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dsp);

		if (desc.mPreset != ReverbPreset::Off) {
			FMOD_REVERB_PROPERTIES props = skReverbPresets[static_cast<byte>(desc.mPreset)];
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, props.DecayTime);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, props.EarlyDelay);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, props.LateDelay);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, props.HFReference);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, props.Diffusion);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, props.WetLevel);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, desc.mDryLevel);
			dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DENSITY, props.Density);
			return dsp;
		}

		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, desc.mDecayTime);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, desc.mEarlyDelay);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, desc.mLateDelay);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, desc.mReferenceFreq);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, desc.mDiffusion);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, desc.mWetLevel);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, desc.mDryLevel);
		dsp->setParameterFloat(FMOD_DSP_SFXREVERB_DENSITY, desc.mDensity);

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_frequency_effect(const FAudioEffectDescriptor::FFrequencyPass::FPass& desc, detail::FrequnecyType type) {

		FMOD::DSP* dsp = nullptr;
		
		if (type == detail::FrequnecyType::Low) {

			mSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsp);

			dsp->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, desc.mCutoff);
			dsp->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, desc.mResonance);

		}
		else if (type == detail::FrequnecyType::High) {

			mSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsp);

			dsp->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, desc.mCutoff);
			dsp->setParameterFloat(FMOD_DSP_HIGHPASS_RESONANCE, desc.mResonance);

		}

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_echo_effect(const FAudioEffectDescriptor::FEcho& desc) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);

		dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, desc.mDelay);
		dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, desc.mFeedback);
		dsp->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, desc.mDryLevel);
		dsp->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, desc.mWetLevel);

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_distortion_effect(const FAudioEffectDescriptor::FDistortion& desc) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dsp);

		dsp->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, desc.mLevel);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_chorus_effect(const FAudioEffectDescriptor::FChorus& desc) {
		
		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dsp);

		dsp->setParameterFloat(FMOD_DSP_CHORUS_RATE, desc.mRate);
		dsp->setParameterFloat(FMOD_DSP_CHORUS_DEPTH, desc.mDepth);
		dsp->setParameterFloat(FMOD_DSP_CHORUS_MIX, desc.mMix);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_flange_effect(const FAudioEffectDescriptor::FFlange& desc) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dsp);

		dsp->setParameterFloat(FMOD_DSP_FLANGE_RATE, desc.mRate);
		dsp->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, desc.mDepth);
		dsp->setParameterFloat(FMOD_DSP_FLANGE_MIX, desc.mMix);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_compressor_effect(const FAudioEffectDescriptor::FCompressor& desc) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_COMPRESSOR, &dsp);

		dsp->setParameterFloat(FMOD_DSP_COMPRESSOR_THRESHOLD, desc.mThreshold);
		dsp->setParameterFloat(FMOD_DSP_COMPRESSOR_RATIO, desc.mRatio);
		dsp->setParameterFloat(FMOD_DSP_COMPRESSOR_ATTACK, desc.mAttack);
		dsp->setParameterFloat(FMOD_DSP_COMPRESSOR_RELEASE, desc.mRelease);
		dsp->setParameterFloat(FMOD_DSP_COMPRESSOR_GAINMAKEUP, desc.mGainMakeup);

		return dsp;

	}

} // namespace lum::ahi