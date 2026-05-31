//========= Copyright (C) 2025-present 3zymek, MIT License ============//
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

		FMOD::System_Create( &mSystem );
		FMOD_INITFLAGS initFlags = translate_init_flags( flags );
		FMOD_RESULT result = mSystem->init( maxChannels, initFlags, nullptr );
		LUM_ASSERT( result == FMOD_OK, "FMOD init failed" );

	}

	void FMODDevice::Finalize( ) {

		mSystem->release( );

	}

	SoundHandle FMODDevice::LoadSound( StringView path, Flags<SoundFlag> flags ) {

		FMOD::Sound* sound = nullptr;
		FMOD_MODE fmodFlags = translate_sound_flags( flags );

		mSystem->createSound( path.data( ), fmodFlags, nullptr, &sound );

		return mSounds.Append( std::move( sound ) );

	}

	void FMODDevice::UnloadSound( SoundHandle& sound ) {

		FMOD::Sound* fmodSound = to_fmod_sound( mSounds[ sound ] );
		fmodSound->release( );
		mSounds.Remove( sound );

	}

	AudioEffectHandle FMODDevice::CreateEffect( const AudioEffectCreateInfo& desc ) {

		AudioEffect effect;

		if (desc.mReverb.mEnabled)
			effect.mDsps.push_back( create_reverb_effect( desc.mReverb ) );

		if (desc.mFreqPass.mLow.mEnabled)
			effect.mDsps.push_back( create_frequency_effect( desc.mFreqPass.mLow, detail::FrequencyType::Low ) );

		if (desc.mFreqPass.mHigh.mEnabled)
			effect.mDsps.push_back( create_frequency_effect( desc.mFreqPass.mHigh, detail::FrequencyType::High ) );

		if (desc.mEcho.mEnabled)
			effect.mDsps.push_back( create_echo_effect( desc.mEcho ) );

		if (desc.mDistortion.mEnabled)
			effect.mDsps.push_back( create_distortion_effect( desc.mDistortion ) );

		if (desc.mChorus.mEnabled)
			effect.mDsps.push_back( create_chorus_effect( desc.mChorus ) );

		if (desc.mFlange.mEnabled)
			effect.mDsps.push_back( create_flange_effect( desc.mFlange ) );

		if (desc.mCompressor.mEnabled)
			effect.mDsps.push_back( create_compressor_effect( desc.mCompressor ) );

		if (desc.mParamEQ.mEnabled)
			effect.mDsps.push_back( create_parameq( desc.mParamEQ ) );


		return mEffects.Append( std::move( effect ) );

	}

	AudioEffectHandle FMODDevice::CreateEffect( ahi::EffectPreset preset ) {

		AudioEffectCreateInfo desc = GetPreset( preset );

		return CreateEffect( desc );

	}

	void FMODDevice::DeleteEffect( AudioEffectHandle& effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		AudioEffect& sfx = mEffects[ effect ];

		for (auto [slot, value] : mChannelGroups.Each( )) {

			FMOD::ChannelGroup* group = to_fmod_channel_group( *value );
			for (auto* dsp : sfx.mDsps) {
				group->removeDSP( to_fmod_dsp( dsp ) );
			}

		}

		for (auto* dsp : sfx.mDsps)
			to_fmod_dsp( dsp )->release( );
		mEffects.Remove( effect );

	}

	void FMODDevice::SetGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group handle" );

		FMOD::ChannelGroup* fmodGroup = to_fmod_channel_group( mChannelGroups[ group ] );
		AudioEffect sfx = mEffects[ effect ];

		for (int32 i = 0; i < sfx.mDsps.size( ); i++) {
			fmodGroup->addDSP( i, to_fmod_dsp( sfx.mDsps[ i ] ) );
		}

	}
	void FMODDevice::SetGroupVolume( ChannelGroupHandle group, float32 volume ) {

		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group" );
		to_fmod_channel_group( mChannelGroups[ group ] )->setVolume( std::clamp( volume, 0.0f, 1.0f ) );

	}
	void FMODDevice::SetGroupPitch( ChannelGroupHandle group, float32 pitch ) {

		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group" );
		to_fmod_channel_group( mChannelGroups[ group ] )->setPitch( std::clamp( pitch, 0.0f, 1.0f ) );

	}
	void FMODDevice::RemoveGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group handle" );

		FMOD::ChannelGroup* fmodGroup = to_fmod_channel_group( mChannelGroups[ group ] );
		AudioEffect sfx = mEffects[ effect ];

		for (int32 i = 0; i < sfx.mDsps.size( ); i++) {
			fmodGroup->removeDSP( to_fmod_dsp( sfx.mDsps[ i ] ) );
		}

	}

	ChannelGroupHandle FMODDevice::CreateChannelGroup( StringView name ) {

		FMOD::ChannelGroup* group;
		mSystem->createChannelGroup( name.data( ), &group );
		return mChannelGroups.Append( std::move( group ) );

	}

	void FMODDevice::PlayOneShot( SoundHandle sound, const SoundPlaybackDescription& desc ) {

		FMOD::Sound* fmodSound = static_cast< FMOD::Sound* >(mSounds[ sound ]);
		FMOD::Channel* channel = nullptr;
		if (desc.mGroup == kDefaultGroup)
			mSystem->playSound( fmodSound, nullptr, false, &channel );
		else
			mSystem->playSound( fmodSound, to_fmod_channel_group( mChannelGroups[ desc.mGroup ] ), false, &channel );
		channel->setVolume( desc.mVolume );
		channel->setPitch( desc.mPitch );

	}

	void FMODDevice::Play( SoundInstance& inst, ChannelGroupHandle group ) {

		LUM_ASSERT( mSounds.Contains( inst.mSound ), "Invalid sound" );

		FMOD::Sound* fmodSound = static_cast< FMOD::Sound* >(mSounds[ inst.mSound.mId ]);
		FMOD::Channel* channel = nullptr;

		mSystem->playSound( fmodSound, nullptr, inst.mFlags.Has( SoundInstanceFlag::Paused ), &channel );
		channel->setVolume( std::clamp( inst.mVolume, 0.0f, 1.0f ) );
		channel->setPitch( std::clamp( inst.mPitch, 0.0f, 1.0f ) );

		if (group != kDefaultGroup) {

			LUM_ASSERT( mChannelGroups.Contains( group ), "Invalid group" );
			channel->setChannelGroup( to_fmod_channel_group( mChannelGroups[ group ] ) );

		}

		inst.mFlags.Set( SoundInstanceFlag::Playing );
		mChannels.insert( { inst.mInstanceId, channel } );

	}

	void FMODDevice::StopAll( ) {

		FMOD::ChannelGroup* master;
		mSystem->getMasterChannelGroup( &master );
		master->stop( );

	}

	void FMODDevice::SetMasterVolume( float32 volume ) {

		FMOD::ChannelGroup* master;
		mSystem->getMasterChannelGroup( &master );
		master->setVolume( std::clamp( volume, 0.0f, 1.0f ) );

	}

	void FMODDevice::Set3DListenerAttributes( const ahi::ListenerAttributes& attrs ) {

		FMOD_VECTOR fmodPos = { attrs.mPosition.mX, attrs.mPosition.mY, attrs.mPosition.mZ };
		FMOD_VECTOR fmodVel = { attrs.mVelocity.mX, attrs.mVelocity.mY, attrs.mVelocity.mZ };
		FMOD_VECTOR fmodForward = { attrs.mForward.mX, attrs.mForward.mY, attrs.mForward.mZ };
		FMOD_VECTOR fmodUp = { attrs.mUp.mX, attrs.mUp.mY, attrs.mUp.mZ };

		mSystem->set3DListenerAttributes( 0, &fmodPos, &fmodVel, &fmodForward, &fmodUp );

	}

	void FMODDevice::Set3DSettings( float32 dopplerScale, float32 distanceFactor, float32 rolloffScale ) {

		mSystem->set3DSettings( dopplerScale, distanceFactor, rolloffScale );

	}

	void FMODDevice::UpdateInstance( SoundInstance& inst ) {

		if (inst.mFlags.Has( SoundInstanceFlag::Play )) {

			Play( inst, inst.mGroup );
			inst.mFlags.Unset( SoundInstanceFlag::Play );

		}

		if (!mChannels.contains( inst.mInstanceId )) return;

		FMOD::Channel* channel = to_fmod_channel( mChannels[ inst.mInstanceId ] );

		bool playing;
		channel->isPlaying( &playing );

		// End streaming
		if (!playing) { mChannels.erase( inst.mInstanceId ); return; }
		if (inst.mFlags.Has( SoundInstanceFlag::Stop )) {
			channel->stop( );
			mChannels.erase( inst.mInstanceId );
			return;
		}

		if (inst.mFlags.Has( SoundInstanceFlag::Kill )) {

		}

		channel->setPaused( inst.mFlags.Has( SoundInstanceFlag::Paused ) );
		if (inst.mFlags.Has( SoundInstanceFlag::Paused )) return;

		Vector3 instPos = inst.mPosition;
		FMOD_VECTOR pos = { instPos.mX, instPos.mY, instPos.mZ };

		channel->setVolume( std::clamp( inst.mVolume, 0.0f, 1.0f ) );
		channel->setPitch( std::clamp( inst.mPitch, 0.0f, 1.0f ) );
		channel->set3DAttributes( &pos, nullptr );
		channel->set3DMinMaxDistance( inst.mMinDistance, inst.mMaxDistance );

		if (inst.mFlags.Has( SoundInstanceFlag::Looped ))
			channel->setMode( FMOD_LOOP_NORMAL );
		else
			channel->setMode( FMOD_LOOP_OFF );

	}

	void FMODDevice::SubmitFrame( ) {

		mSystem->update( );

	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	FMOD_MODE FMODDevice::translate_sound_flags( Flags<SoundFlag> flags ) {

		FMOD_MODE mode = 0;

		if (flags.Has( SoundFlag::Default ))                mode |= FMOD_DEFAULT;
		if (flags.Has( SoundFlag::CreateStream ))           mode |= FMOD_CREATESTREAM;
		if (flags.Has( SoundFlag::CreateSample ))           mode |= FMOD_CREATESAMPLE;
		if (flags.Has( SoundFlag::CreateCompressedSample )) mode |= FMOD_CREATECOMPRESSEDSAMPLE;
		if (flags.Has( SoundFlag::NonBlocking ))            mode |= FMOD_NONBLOCKING;
		if (flags.Has( SoundFlag::Unique ))                 mode |= FMOD_UNIQUE;
		if (flags.Has( SoundFlag::LowMem ))                 mode |= FMOD_LOWMEM;
		if (flags.Has( SoundFlag::Spatial3D ))              mode |= FMOD_3D;
		if (flags.Has( SoundFlag::Flat2D ))                 mode |= FMOD_2D;
		if (flags.Has( SoundFlag::LinearRolloff ))          mode |= FMOD_3D_LINEARROLLOFF;
		if (flags.Has( SoundFlag::InverseRolloff ))         mode |= FMOD_3D_INVERSEROLLOFF;

		return mode;

	}

	FMOD_INITFLAGS FMODDevice::translate_init_flags( Flags<InitFlag> flags ) {
		FMOD_INITFLAGS result = 0;
		if (flags.Has( InitFlag::Normal ))             result |= FMOD_INIT_NORMAL;
		if (flags.Has( InitFlag::RightHanded3D ))      result |= FMOD_INIT_3D_RIGHTHANDED;
		if (flags.Has( InitFlag::DistanceLowpass ))    result |= FMOD_INIT_CHANNEL_LOWPASS;
		if (flags.Has( InitFlag::DistanceFilter ))     result |= FMOD_INIT_CHANNEL_DISTANCEFILTER;
		if (flags.Has( InitFlag::ProfilerEnable ))     result |= FMOD_INIT_PROFILE_ENABLE;
		if (flags.Has( InitFlag::Vol0BecomesVirtual )) result |= FMOD_INIT_VOL0_BECOMES_VIRTUAL;
		return result;
	}

	FMOD::DSP* FMODDevice::create_reverb_effect( const AudioEffectCreateInfo::Reverb& desc ) {

		FMOD::DSP* dsp = nullptr;
		mSystem->createDSPByType( FMOD_DSP_TYPE_SFXREVERB, &dsp );

		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DECAYTIME, desc.mDecayTime );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_EARLYDELAY, desc.mEarlyDelay );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_LATEDELAY, desc.mLateDelay );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_HFREFERENCE, desc.mReferenceFreq );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DIFFUSION, desc.mDiffusion );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_WETLEVEL, desc.mWetLevel );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DRYLEVEL, desc.mDryLevel );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DENSITY, desc.mDensity );

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_frequency_effect( const AudioEffectCreateInfo::FrequencyPass::Pass& desc, detail::FrequencyType type ) {

		FMOD::DSP* dsp = nullptr;

		if (type == detail::FrequencyType::Low) {

			mSystem->createDSPByType( FMOD_DSP_TYPE_LOWPASS, &dsp );

			dsp->setParameterFloat( FMOD_DSP_LOWPASS_CUTOFF, desc.mCutoff );
			dsp->setParameterFloat( FMOD_DSP_LOWPASS_RESONANCE, desc.mResonance );

		}
		else if (type == detail::FrequencyType::High) {

			mSystem->createDSPByType( FMOD_DSP_TYPE_HIGHPASS, &dsp );

			dsp->setParameterFloat( FMOD_DSP_HIGHPASS_CUTOFF, desc.mCutoff );
			dsp->setParameterFloat( FMOD_DSP_HIGHPASS_RESONANCE, desc.mResonance );

		}

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_echo_effect( const AudioEffectCreateInfo::Echo& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_ECHO, &dsp );

		dsp->setParameterFloat( FMOD_DSP_ECHO_DELAY, desc.mDelay );
		dsp->setParameterFloat( FMOD_DSP_ECHO_FEEDBACK, desc.mFeedback );
		dsp->setParameterFloat( FMOD_DSP_ECHO_DRYLEVEL, desc.mDryLevel );
		dsp->setParameterFloat( FMOD_DSP_ECHO_WETLEVEL, desc.mWetLevel );

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_distortion_effect( const AudioEffectCreateInfo::Distortion& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_DISTORTION, &dsp );

		dsp->setParameterFloat( FMOD_DSP_DISTORTION_LEVEL, desc.mLevel );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_chorus_effect( const AudioEffectCreateInfo::Chorus& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_CHORUS, &dsp );

		dsp->setParameterFloat( FMOD_DSP_CHORUS_RATE, desc.mRate );
		dsp->setParameterFloat( FMOD_DSP_CHORUS_DEPTH, desc.mDepth );
		dsp->setParameterFloat( FMOD_DSP_CHORUS_MIX, desc.mMix );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_flange_effect( const AudioEffectCreateInfo::Flange& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_FLANGE, &dsp );

		dsp->setParameterFloat( FMOD_DSP_FLANGE_RATE, desc.mRate );
		dsp->setParameterFloat( FMOD_DSP_FLANGE_DEPTH, desc.mDepth );
		dsp->setParameterFloat( FMOD_DSP_FLANGE_MIX, desc.mMix );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_compressor_effect( const AudioEffectCreateInfo::Compressor& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_COMPRESSOR, &dsp );

		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_THRESHOLD, desc.mThreshold );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_RATIO, desc.mRatio );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_ATTACK, desc.mAttack );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_RELEASE, desc.mRelease );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_GAINMAKEUP, desc.mGainMakeup );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_parameq( const AudioEffectCreateInfo::ParamEQ& desc ) {

		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType( FMOD_DSP_TYPE_PARAMEQ, &dsp );

		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_CENTER, desc.mCenter );
		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_BANDWIDTH, desc.mBandwidth );
		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_GAIN, desc.mGain );

		return dsp;

	}

} // namespace lum::ahi