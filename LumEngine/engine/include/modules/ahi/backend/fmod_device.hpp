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
		void Initialize( int32, Flags<InitFlag> ) override;
		void Finalize( ) override;

		// Sound loading
		SoundHandle LoadSound( StringView, Flags<SoundFlag> ) override;
		void        UnloadSound( SoundHandle& ) override;

		// Effects
		AudioEffectHandle CreateEffect( const AudioEffectCreateInfo& ) override;
		AudioEffectHandle CreateEffect( ahi::EffectPreset ) override;
		void              DeleteEffect( AudioEffectHandle& ) override;

		// Channel groups
		ChannelGroupHandle CreateChannelGroup( StringView ) override;
		void SetGroupEffect( ChannelGroupHandle, AudioEffectHandle ) override;
		void SetGroupVolume( ChannelGroupHandle, float32 ) override;
		void SetGroupPitch( ChannelGroupHandle, float32 ) override;
		void RemoveGroupEffect( ChannelGroupHandle, AudioEffectHandle ) override;

		// Playback
		void PlayOneShot( SoundHandle, const PlaybackDescriptor& ) override;
		void Play( SoundInstance&, ChannelGroupHandle ) override;
		void StopAll( ) override;

		// Global settings
		void SetMasterVolume( float32 ) override;
		void Set3DListenerAttributes( const ahi::ListenerAttributes& ) override;
		void Set3DSettings( float32, float32, float32 ) override;

		// Per-frame
		void UpdateInstance( SoundInstance& ) override;
		void EndFrame( ) override;

	private:

		FMOD::System* mSystem = nullptr;

		// Handle casts
		FMOD::Sound* to_fmod_sound( vptr ptr )    const noexcept { return static_cast< FMOD::Sound* >(ptr); }
		FMOD::Channel* to_fmod_channel( vptr ptr )  const noexcept { return static_cast< FMOD::Channel* >(ptr); }
		FMOD::ChannelGroup* to_fmod_chgroup( vptr ptr )  const noexcept { return static_cast< FMOD::ChannelGroup* >(ptr); }
		FMOD::DSP* to_fmod_dsp( vptr ptr )      const noexcept { return static_cast< FMOD::DSP* >(ptr); }

		// Flag translation
		static FMOD_MODE      translate_sound_flags( Flags<SoundFlag> );
		static FMOD_INITFLAGS translate_init_flags( Flags<InitFlag> );

		// DSP factory methods
		FMOD::DSP* create_reverb_effect( const AudioEffectCreateInfo::Reverb& );
		FMOD::DSP* create_frequency_effect( const AudioEffectCreateInfo::FrequencyPass::Pass&, detail::FrequnecyType );
		FMOD::DSP* create_echo_effect( const AudioEffectCreateInfo::Echo& );
		FMOD::DSP* create_distortion_effect( const AudioEffectCreateInfo::Distortion& );
		FMOD::DSP* create_chorus_effect( const AudioEffectCreateInfo::Chorus& );
		FMOD::DSP* create_flange_effect( const AudioEffectCreateInfo::Flange& );
		FMOD::DSP* create_compressor_effect( const AudioEffectCreateInfo::Compressor& );
		FMOD::DSP* create_parameq( const AudioEffectCreateInfo::ParamEQ& );
	};

} // namespace lum::ahi::fmod