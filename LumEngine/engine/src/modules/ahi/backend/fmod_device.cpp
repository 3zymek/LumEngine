//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: FMOD implementation of Audio Hardware Interface.
//
//=============================================================================//

#include "ahi/backend/fmod_device.hpp"

namespace lum::ahi::fmod {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void FMODDevice::Initialize( int32 maxChannels, Flags<InitFlag> flags ) {

		FMOD::System_Create(&mSystem);
		FMOD_INITFLAGS initFlags = translate_init_flags(flags);
		FMOD_RESULT result = mSystem->init(maxChannels, initFlags, nullptr);
		LUM_ASSERT(result == FMOD_OK, "FMOD init failed");

	}

	void FMODDevice::Finalize( ) {

		mSystem->release();

	}

	SoundHandle FMODDevice::LoadSound( StringView path, Flags<SoundFlag> flags ) {
		
		FMOD::Sound* sound = nullptr;

		FMOD_MODE fmodFlags = translate_sound_flags(flags);

		mSystem->createSound(path.data(), fmodFlags, nullptr, &sound);

		return mSounds.Append(std::move(sound));

	}

	void FMODDevice::UnloadSound( SoundHandle& sound ) {

		FMOD::Sound* fmodSound = to_fmod_sound(mSounds[sound]);
		fmodSound->release();
		mSounds.Remove(sound);

	}

	AudioEffectHandle FMODDevice::CreateEffect( const FAudioEffectDescriptor& desc ) {
		
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

	AudioEffectHandle FMODDevice::CreateEffect(ahi::EffectPreset preset) {

		FAudioEffectDescriptor desc = detail::gkEffectPresetLookup[ToUnderlyingEnum(preset)];

		return CreateEffect(desc);

	}

	void FMODDevice::DeleteEffect( AudioEffectHandle& effect ) {

		LUM_RETURN_IF(!IsValid(effect), LUM_SEV_WARN, "Invalid effect handle");

		FAudioEffect& sfx = mEffects[effect];

		for (auto [slot, value] : mChannelGroups.Each()) {
			
			FMOD::ChannelGroup* group = to_fmod_chgroup(*value);
			for (auto* dsp : sfx.mDSPs) {
				group->removeDSP(to_fmod_dsp(dsp));
			}
			
		}

		for (auto* dsp : sfx.mDSPs)
			to_fmod_dsp(dsp)->release();

		mEffects.Remove(effect);
	}

	void FMODDevice::AddGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {
		
		LUM_RETURN_IF(!IsValid(effect), LUM_SEV_WARN, "Invalid effect handle");
		LUM_RETURN_IF(!IsValid(group), LUM_SEV_WARN, "Invalid group handle");

		FMOD::ChannelGroup* fmodGroup = to_fmod_chgroup(mChannelGroups[group]);
		FAudioEffect sfx = mEffects[effect];

		for (int32 i = 0; i < sfx.mDSPs.size(); i++) {
			fmodGroup->addDSP(i, to_fmod_dsp(sfx.mDSPs[i]));
		}

	}
	void FMODDevice::RemoveGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {

		LUM_RETURN_IF(!IsValid(effect), LUM_SEV_WARN, "Invalid effect handle");
		LUM_RETURN_IF(!IsValid(group), LUM_SEV_WARN, "Invalid group handle");

		FMOD::ChannelGroup* fmodGroup = to_fmod_chgroup(mChannelGroups[group]);
		FAudioEffect sfx = mEffects[effect];

		for (int32 i = 0; i < sfx.mDSPs.size(); i++) {
			fmodGroup->removeDSP(to_fmod_dsp(sfx.mDSPs[i]));
		}

	}

	ChannelGroupHandle FMODDevice::CreateChannelGroup( StringView name ) {

		FMOD::ChannelGroup* group;
		mSystem->createChannelGroup(name.data(), &group);

		return mChannelGroups.Append(std::move(group));

	}
	
	void FMODDevice::PlayOneShot( SoundHandle sound, const FPlaybackDescriptor& desc ) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[sound]);
		FMOD::Channel* channel = nullptr;
		if (desc.mGroup == gDefaultGroup)
			mSystem->playSound(fmodSound, nullptr, false, &channel);
		else
			mSystem->playSound(fmodSound, to_fmod_chgroup(mChannelGroups[desc.mGroup]), false, &channel);
		channel->setVolume(desc.mVolume);
		channel->setPitch(desc.mPitch);

	}

	void FMODDevice::Play( FSoundInstance& inst, ChannelGroupHandle group ) {

		LUM_ASSERT(mSounds.Contains(inst.mSound), "Invalid sound");

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(mSounds[inst.mSound.mID]);
		FMOD::Channel* channel = nullptr;

		mSystem->playSound(fmodSound, nullptr, inst.bPaused, &channel);
		channel->setVolume(inst.mVolume);
		channel->setPitch(inst.mPitch);

		if (group != gDefaultGroup) {

			LUM_ASSERT(mChannelGroups.Contains(group), "Invalid group");
			channel->setChannelGroup(to_fmod_chgroup(mChannelGroups[group]));
		
		}

		inst.bPlaying = true;
		mChannels.insert({ inst.mInstanceID, channel });

	}

	void FMODDevice::StopAll( ) {

		for (auto& [key, value] : mChannels) {
			
			to_fmod_channel(value)->stop();

		}

	}

	void FMODDevice::SetMasterVolume( float32 volume ) {
		
		FMOD::ChannelGroup* master;
		mSystem->getMasterChannelGroup(&master);
		master->setVolume(volume);
		
	}

	void FMODDevice::Set3DListenerAttributes( glm::vec3 pos, glm::vec3 vel, glm::vec3 forward, glm::vec3 up ) {

		FMOD_VECTOR fmodPos = { pos.x, pos.y, pos.z };
		FMOD_VECTOR fmodVel = { vel.x, vel.y, vel.z };
		FMOD_VECTOR fmodForward = { forward.x, forward.y, forward.z };
		FMOD_VECTOR fmodUp = { up.x, up.y, up.z };

		mSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);

	}

	void FMODDevice::Set3DSettings( float32 dopplerScale, float32 distanceFactor, float32 rolloffScale ) {
		
		mSystem->set3DSettings(dopplerScale, distanceFactor, rolloffScale);

	}

	void FMODDevice::Update( FSoundInstance& instance ) {

		auto it = mChannels.find(instance.mInstanceID);
		LUM_RETURN_IF(it == mChannels.end(), LUM_SEV_WARN, "Instance's not playing");
		if (it == mChannels.end()) return;
		FMOD::Channel* channel = to_fmod_channel(it->second);

		bool playing;
		channel->isPlaying(&playing);

		// End streaming
		if (!playing) { mChannels.erase(instance.mInstanceID); return; }
		if (!instance.bPlaying) {
			channel->stop();
			mChannels.erase(instance.mInstanceID);
			return;
		}

		channel->setPaused(instance.bPaused);
		if (instance.bPaused) return;

		glm::vec3 instPos = instance.mPosition;
		FMOD_VECTOR pos = { instPos.x, instPos.y, instPos.z };

		channel->setVolume(instance.mVolume);
		channel->setPitch(instance.mPitch);
		channel->set3DAttributes(&pos, nullptr);
		channel->set3DMinMaxDistance(instance.mMinDistance, instance.mMaxDistance);

		if (instance.bLooped)
			channel->setMode(FMOD_LOOP_NORMAL);
		else
			channel->setMode(FMOD_LOOP_OFF);

		mSystem->update();

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	FMOD_MODE FMODDevice::translate_sound_flags( Flags<SoundFlag> flags ) {

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

	FMOD_INITFLAGS FMODDevice::translate_init_flags( Flags<InitFlag> flags ) {
		FMOD_INITFLAGS result = 0;
		if (flags.Has(InitFlag::Normal))             result |= FMOD_INIT_NORMAL;
		if (flags.Has(InitFlag::RightHanded3D))      result |= FMOD_INIT_3D_RIGHTHANDED;
		if (flags.Has(InitFlag::DistanceLowpass))    result |= FMOD_INIT_CHANNEL_LOWPASS;
		if (flags.Has(InitFlag::DistanceFilter))     result |= FMOD_INIT_CHANNEL_DISTANCEFILTER;
		if (flags.Has(InitFlag::ProfilerEnable))     result |= FMOD_INIT_PROFILE_ENABLE;
		if (flags.Has(InitFlag::Vol0BecomesVirtual)) result |= FMOD_INIT_VOL0_BECOMES_VIRTUAL;
		return result;
	}

	FMOD::DSP* FMODDevice::create_reverb_effect( const FAudioEffectDescriptor::FReverb& desc ) {

		FMOD::DSP* dsp = nullptr;
		mSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dsp);

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

	FMOD::DSP* FMODDevice::create_frequency_effect( const FAudioEffectDescriptor::FFrequencyPass::FPass& desc, detail::FrequnecyType type ) {

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

	FMOD::DSP* FMODDevice::create_echo_effect( const FAudioEffectDescriptor::FEcho& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);

		dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, desc.mDelay);
		dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, desc.mFeedback);
		dsp->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, desc.mDryLevel);
		dsp->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, desc.mWetLevel);

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_distortion_effect( const FAudioEffectDescriptor::FDistortion& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dsp);

		dsp->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, desc.mLevel);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_chorus_effect( const FAudioEffectDescriptor::FChorus& desc ) {
		
		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dsp);

		dsp->setParameterFloat(FMOD_DSP_CHORUS_RATE, desc.mRate);
		dsp->setParameterFloat(FMOD_DSP_CHORUS_DEPTH, desc.mDepth);
		dsp->setParameterFloat(FMOD_DSP_CHORUS_MIX, desc.mMix);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_flange_effect( const FAudioEffectDescriptor::FFlange& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dsp);

		dsp->setParameterFloat(FMOD_DSP_FLANGE_RATE, desc.mRate);
		dsp->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, desc.mDepth);
		dsp->setParameterFloat(FMOD_DSP_FLANGE_MIX, desc.mMix);

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_compressor_effect( const FAudioEffectDescriptor::FCompressor& desc ) {

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