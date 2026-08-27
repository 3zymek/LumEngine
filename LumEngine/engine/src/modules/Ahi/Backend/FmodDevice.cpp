//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: FMOD implementation of Audio Hardware Interface.
//
//=============================================================================//

#include "Ahi/Backend/FmodDevice.hpp"
#include "Platform/FileSystem/Filesystem.hpp"

namespace lum::ahi::fmod {

	//=======================================================//
		// Public
	//=======================================================//

	void FMODDevice::Initialize( int32 maxChannels, Flags<InitFlag> flags ) {

		FMOD::System_Create( &m_System );
		FMOD_INITFLAGS initFlags = translate_init_flags( flags );
		FMOD_RESULT result = m_System->init( maxChannels, initFlags, nullptr );
		LUM_ASSERT( result == FMOD_OK, "FMOD init failed" );

	}

	void FMODDevice::Finalize( ) {

		m_System->release( );

	}

	SoundHandle FMODDevice::LoadSound( Path path, Flags<SoundFlag> flags ) {

		FMOD::Sound* sound = nullptr;
		FMOD_MODE fmodFlags = translate_sound_flags( flags );

		m_System->createSound( path.ToString( ).data( ), fmodFlags, nullptr, &sound );

		return m_Sounds.Append( std::move( sound ) );

	}

	void FMODDevice::UnloadSound( SoundHandle& sound ) {

		FMOD::Sound* fmodSound = to_fmod_sound( m_Sounds[ sound ] );
		fmodSound->release( );
		m_Sounds.Remove( sound );

	}

	AudioEffectHandle FMODDevice::CreateEffect( const AudioEffectCreateInfo& desc ) {

		AudioEffect effect{};

		if (desc.m_Reverb.m_Enabled)
			effect.m_Dsps.push_back( create_reverb_effect( desc.m_Reverb ) );

		if (desc.m_FreqPass.m_Low.m_Enabled)
			effect.m_Dsps.push_back( create_frequency_effect( desc.m_FreqPass.m_Low, detail::FrequencyType::Low ) );

		if (desc.m_FreqPass.m_High.m_Enabled)
			effect.m_Dsps.push_back( create_frequency_effect( desc.m_FreqPass.m_High, detail::FrequencyType::High ) );

		if (desc.m_Echo.m_Enabled)
			effect.m_Dsps.push_back( create_echo_effect( desc.m_Echo ) );

		if (desc.m_Distortion.m_Enabled)
			effect.m_Dsps.push_back( create_distortion_effect( desc.m_Distortion ) );

		if (desc.m_Chorus.m_Enabled)
			effect.m_Dsps.push_back( create_chorus_effect( desc.m_Chorus ) );

		if (desc.m_Flange.m_Enabled)
			effect.m_Dsps.push_back( create_flange_effect( desc.m_Flange ) );

		if (desc.m_Compressor.m_Enabled)
			effect.m_Dsps.push_back( create_compressor_effect( desc.m_Compressor ) );

		if (desc.m_ParamEQ.m_Enabled)
			effect.m_Dsps.push_back( create_parameq( desc.m_ParamEQ ) );


		return m_Effects.Append( std::move( effect ) );

	}

	AudioEffectHandle FMODDevice::CreateEffect( ahi::EffectPreset preset ) {

		AudioEffectCreateInfo desc = GetPreset( preset );

		return CreateEffect( desc );

	}

	void FMODDevice::DeleteEffect( AudioEffectHandle& effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		AudioEffect& sfx = m_Effects[ effect ];

		for (auto [slot, value] : m_ChannelGroups.Each( )) {

			FMOD::ChannelGroup* group = to_fmod_channel_group( *value );
			for (auto* dsp : sfx.m_Dsps) {
				group->removeDSP( to_fmod_dsp( dsp ) );
			}

		}

		for (auto* dsp : sfx.m_Dsps)
			to_fmod_dsp( dsp )->release( );
		m_Effects.Remove( effect );

	}

	void FMODDevice::SetGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group handle" );

		FMOD::ChannelGroup* fmodGroup = to_fmod_channel_group( m_ChannelGroups[ group ] );
		AudioEffect sfx = m_Effects[ effect ];

		for (int32 i = 0; i < sfx.m_Dsps.size( ); i++) {
			fmodGroup->addDSP( i, to_fmod_dsp( sfx.m_Dsps[ i ] ) );
		}

	}
	void FMODDevice::SetGroupVolume( ChannelGroupHandle group, float32 volume ) {

		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group" );
		to_fmod_channel_group( m_ChannelGroups[ group ] )->setVolume( std::clamp( volume, 0.0f, 1.0f ) );

	}
	void FMODDevice::SetGroupPitch( ChannelGroupHandle group, float32 pitch ) {

		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group" );
		to_fmod_channel_group( m_ChannelGroups[ group ] )->setPitch( std::clamp( pitch, 0.0f, 1.0f ) );

	}
	void FMODDevice::RemoveGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) {

		LUM_RETURN_IF( !IsValid( effect ), LUM_SEV_WARN, "Invalid effect handle" );
		LUM_RETURN_IF( !IsValid( group ), LUM_SEV_WARN, "Invalid group handle" );

		FMOD::ChannelGroup* fmodGroup = to_fmod_channel_group( m_ChannelGroups[ group ] );
		AudioEffect sfx = m_Effects[ effect ];

		for (int32 i = 0; i < sfx.m_Dsps.size( ); i++) {
			fmodGroup->removeDSP( to_fmod_dsp( sfx.m_Dsps[ i ] ) );
		}

	}

	ChannelGroupHandle FMODDevice::CreateChannelGroup( StringView name ) {

		FMOD::ChannelGroup* group{};
		m_System->createChannelGroup( name.data( ), &group );
		return m_ChannelGroups.Append( std::move( group ) );

	}

	void FMODDevice::PlayOneShot( SoundHandle sound, const SoundPlaybackDescription& desc ) {

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(m_Sounds[ sound ]);
		FMOD::Channel* channel = nullptr;
		if (desc.m_Group == k_DefaultGroup)
			m_System->playSound( fmodSound, nullptr, false, &channel );
		else
			m_System->playSound( fmodSound, to_fmod_channel_group( m_ChannelGroups[ desc.m_Group ] ), false, &channel );
		channel->setVolume( desc.m_Volume );
		channel->setPitch( desc.m_Pitch );

	}

	void FMODDevice::Play( SoundInstance& inst, ChannelGroupHandle group ) {

		LUM_ASSERT( m_Sounds.Contains( inst.m_Sound ), "Invalid sound" );

		FMOD::Sound* fmodSound = static_cast<FMOD::Sound*>(m_Sounds[ inst.m_Sound.m_Id ]);
		FMOD::Channel* channel = nullptr;

		m_System->playSound( fmodSound, nullptr, inst.m_Flags.Has( SoundInstanceFlag::Paused ), &channel );
		channel->setVolume( std::clamp( inst.m_Volume, 0.0f, 1.0f ) );
		channel->setPitch( std::clamp( inst.m_Pitch, 0.0f, 1.0f ) );

		if (group != k_DefaultGroup) {

			LUM_ASSERT( m_ChannelGroups.Contains( group ), "Invalid group" );
			channel->setChannelGroup( to_fmod_channel_group( m_ChannelGroups[ group ] ) );

		}

		inst.m_Flags.Set( SoundInstanceFlag::Playing );
		m_Channels.insert( { inst.m_InstanceId, channel } );

	}

	void FMODDevice::StopAll( ) {

		FMOD::ChannelGroup* master;
		m_System->getMasterChannelGroup( &master );
		master->stop( );

	}

	void FMODDevice::SetMasterVolume( float32 volume ) {

		FMOD::ChannelGroup* master;
		m_System->getMasterChannelGroup( &master );
		master->setVolume( std::clamp( volume, 0.0f, 1.0f ) );

	}

	void FMODDevice::Set3DListenerAttributes( const ahi::ListenerAttributes& attrs ) {

		FMOD_VECTOR fmodPos = { attrs.m_Position.m_X, attrs.m_Position.m_Y, attrs.m_Position.m_Z };
		FMOD_VECTOR fmodVel = { attrs.m_Velocity.m_X, attrs.m_Velocity.m_Y, attrs.m_Velocity.m_Z };
		FMOD_VECTOR fmodForward = { attrs.m_Forward.m_X, attrs.m_Forward.m_Y, attrs.m_Forward.m_Z };
		FMOD_VECTOR fmodUp = { attrs.m_Up.m_X, attrs.m_Up.m_Y, attrs.m_Up.m_Z };

		m_System->set3DListenerAttributes( 0, &fmodPos, &fmodVel, &fmodForward, &fmodUp );

	}

	void FMODDevice::Set3DSettings( float32 dopplerScale, float32 distanceFactor, float32 rolloffScale ) {

		m_System->set3DSettings( dopplerScale, distanceFactor, rolloffScale );

	}

	void FMODDevice::UpdateInstance( SoundInstance& inst ) {

		if (inst.m_Flags.Has( SoundInstanceFlag::Play )) {

			Play( inst, inst.m_Group );
			inst.m_Flags.Unset( SoundInstanceFlag::Play );

		}

		if (!m_Channels.contains( inst.m_InstanceId )) return;

		FMOD::Channel* channel = to_fmod_channel( m_Channels[ inst.m_InstanceId ] );

		bool playing{};
		channel->isPlaying( &playing );

		// End streaming
		if (!playing) { m_Channels.erase( inst.m_InstanceId ); return; }
		if (inst.m_Flags.Has( SoundInstanceFlag::Stop )) {
			channel->stop( );
			m_Channels.erase( inst.m_InstanceId );
			return;
		}

		if (inst.m_Flags.Has( SoundInstanceFlag::Kill )) {

		}

		channel->setPaused( inst.m_Flags.Has( SoundInstanceFlag::Paused ) );
		if (inst.m_Flags.Has( SoundInstanceFlag::Paused )) return;

		Vector3 instPos = inst.m_Position;
		FMOD_VECTOR pos = { instPos.m_X, instPos.m_Y, instPos.m_Z };

		channel->setVolume( std::clamp( inst.m_Volume, 0.0f, 1.0f ) );
		channel->setPitch( std::clamp( inst.m_Pitch, 0.0f, 1.0f ) );
		channel->set3DAttributes( &pos, nullptr );
		channel->set3DMinMaxDistance( inst.m_MinDistance, inst.m_MaxDistance );

		if (inst.m_Flags.Has( SoundInstanceFlag::Looped ))
			channel->setMode( FMOD_LOOP_NORMAL );
		else
			channel->setMode( FMOD_LOOP_OFF );

	}

	void FMODDevice::SubmitFrame( ) {

		m_System->update( );

	}



	//=======================================================//
		// Private
	//=======================================================//

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
		m_System->createDSPByType( FMOD_DSP_TYPE_SFXREVERB, &dsp );

		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DECAYTIME, desc.m_DecayTime );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_EARLYDELAY, desc.m_EarlyDelay );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_LATEDELAY, desc.m_LateDelay );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_HFREFERENCE, desc.m_ReferenceFreq );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DIFFUSION, desc.m_Diffusion );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_WETLEVEL, desc.m_WetLevel );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DRYLEVEL, desc.m_DryLevel );
		dsp->setParameterFloat( FMOD_DSP_SFXREVERB_DENSITY, desc.m_Density );

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_frequency_effect( const AudioEffectCreateInfo::FrequencyPass::Pass& desc, detail::FrequencyType type ) {

		FMOD::DSP* dsp = nullptr;

		if (type == detail::FrequencyType::Low) {

			m_System->createDSPByType( FMOD_DSP_TYPE_LOWPASS, &dsp );

			dsp->setParameterFloat( FMOD_DSP_LOWPASS_CUTOFF, desc.m_Cutoff );
			dsp->setParameterFloat( FMOD_DSP_LOWPASS_RESONANCE, desc.m_Resonance );

		}
		else if (type == detail::FrequencyType::High) {

			m_System->createDSPByType( FMOD_DSP_TYPE_HIGHPASS, &dsp );

			dsp->setParameterFloat( FMOD_DSP_HIGHPASS_CUTOFF, desc.m_Cutoff );
			dsp->setParameterFloat( FMOD_DSP_HIGHPASS_RESONANCE, desc.m_Resonance );

		}

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_echo_effect( const AudioEffectCreateInfo::Echo& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_ECHO, &dsp );

		dsp->setParameterFloat( FMOD_DSP_ECHO_DELAY, desc.m_Delay );
		dsp->setParameterFloat( FMOD_DSP_ECHO_FEEDBACK, desc.m_Feedback );
		dsp->setParameterFloat( FMOD_DSP_ECHO_DRYLEVEL, desc.m_DryLevel );
		dsp->setParameterFloat( FMOD_DSP_ECHO_WETLEVEL, desc.m_WetLevel );

		return dsp;
	}

	FMOD::DSP* FMODDevice::create_distortion_effect( const AudioEffectCreateInfo::Distortion& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_DISTORTION, &dsp );

		dsp->setParameterFloat( FMOD_DSP_DISTORTION_LEVEL, desc.m_Level );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_chorus_effect( const AudioEffectCreateInfo::Chorus& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_CHORUS, &dsp );

		dsp->setParameterFloat( FMOD_DSP_CHORUS_RATE, desc.m_Rate );
		dsp->setParameterFloat( FMOD_DSP_CHORUS_DEPTH, desc.m_Depth );
		dsp->setParameterFloat( FMOD_DSP_CHORUS_MIX, desc.m_Mix );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_flange_effect( const AudioEffectCreateInfo::Flange& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_FLANGE, &dsp );

		dsp->setParameterFloat( FMOD_DSP_FLANGE_RATE, desc.m_Rate );
		dsp->setParameterFloat( FMOD_DSP_FLANGE_DEPTH, desc.m_Depth );
		dsp->setParameterFloat( FMOD_DSP_FLANGE_MIX, desc.m_Mix );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_compressor_effect( const AudioEffectCreateInfo::Compressor& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_COMPRESSOR, &dsp );

		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_THRESHOLD, desc.m_Threshold );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_RATIO, desc.m_Ratio );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_ATTACK, desc.m_Attack );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_RELEASE, desc.m_Release );
		dsp->setParameterFloat( FMOD_DSP_COMPRESSOR_GAINMAKEUP, desc.m_GainMakeup );

		return dsp;

	}

	FMOD::DSP* FMODDevice::create_parameq( const AudioEffectCreateInfo::ParamEQ& desc ) {

		FMOD::DSP* dsp = nullptr;

		m_System->createDSPByType( FMOD_DSP_TYPE_PARAMEQ, &dsp );

		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_CENTER, desc.m_Center );
		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_BANDWIDTH, desc.m_Bandwidth );
		dsp->setParameterFloat( FMOD_DSP_PARAMEQ_GAIN, desc.m_Gain );

		return dsp;

	}

} // namespace lum::ahi