//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: FMOD implementation of Audio Hardware Interface.
//
//=============================================================================//
#pragma once
#include "ahi/core/ahi_device.hpp"

namespace lum::ahi::fmod {

	class FMODDevice : public AudioDevice {
	public:

		// Lifecycle
		void Initialize( int32 maxChannels, Flags<InitFlag> flags ) override;
		void Finalize( ) override;

		// Sound loading
		SoundHandle LoadSound( StringView path, Flags<SoundFlag> flags ) override;
		void        UnloadSound( SoundHandle& sound ) override;

		// Effects
		AudioEffectHandle CreateEffect( const AudioEffectCreateInfo& desc ) override;
		AudioEffectHandle CreateEffect( ahi::EffectPreset preset ) override;
		void              DeleteEffect( AudioEffectHandle& effect ) override;

		// Channel groups
		void SetGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) override;
		void SetGroupVolume( ChannelGroupHandle group, float32 volume ) override;
		void SetGroupPitch( ChannelGroupHandle group, float32 pitch ) override;
		void RemoveGroupEffect( ChannelGroupHandle group, AudioEffectHandle effect ) override;
		ChannelGroupHandle CreateChannelGroup( StringView name ) override;

		// Playback
		void PlayOneShot( SoundHandle sound, const SoundPlaybackDescription& desc ) override;
		void Play( SoundInstance& inst, ChannelGroupHandle group ) override;
		void StopAll( ) override;

		// Global settings
		void SetMasterVolume( float32 volume ) override;
		void Set3DListenerAttributes( const ahi::ListenerAttributes& attrs ) override;
		void Set3DSettings( float32 dopplerScale, float32 distanceFactor, float32 rolloffScale ) override;

		// Per-frame
		void UpdateInstance( SoundInstance& inst ) override;
		void SubmitFrame( ) override;

	private:

		FMOD::System* mSystem = nullptr;

		// Handle casts
		FMOD::Sound* to_fmod_sound( vptr ptr ) const noexcept { return static_cast< FMOD::Sound* >(ptr); }
		FMOD::Channel* to_fmod_channel( vptr ptr ) const noexcept { return static_cast< FMOD::Channel* >(ptr); }
		FMOD::ChannelGroup* to_fmod_channel_group( vptr ptr )  const noexcept { return static_cast< FMOD::ChannelGroup* >(ptr); }
		FMOD::DSP* to_fmod_dsp( vptr ptr ) const noexcept { return static_cast< FMOD::DSP* >(ptr); }

		// Flag translation
		static FMOD_MODE      translate_sound_flags( Flags<SoundFlag> flags );
		static FMOD_INITFLAGS translate_init_flags( Flags<InitFlag> flags );

		// DSP factory methods
		FMOD::DSP* create_reverb_effect( const AudioEffectCreateInfo::Reverb& desc );
		FMOD::DSP* create_frequency_effect( const AudioEffectCreateInfo::FrequencyPass::Pass&, detail::FrequencyType type );
		FMOD::DSP* create_echo_effect( const AudioEffectCreateInfo::Echo& desc );
		FMOD::DSP* create_distortion_effect( const AudioEffectCreateInfo::Distortion& desc );
		FMOD::DSP* create_chorus_effect( const AudioEffectCreateInfo::Chorus& desc );
		FMOD::DSP* create_flange_effect( const AudioEffectCreateInfo::Flange& desc );
		FMOD::DSP* create_compressor_effect( const AudioEffectCreateInfo::Compressor& desc );
		FMOD::DSP* create_parameq( const AudioEffectCreateInfo::ParamEQ& desc );
	};

} // namespace lum::ahi::fmod