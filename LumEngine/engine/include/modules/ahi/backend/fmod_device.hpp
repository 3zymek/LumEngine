//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: FMOD implementation of Audio Hardware Interface.
//
//=============================================================================//
#pragma once

#include "ahi/core/ahi_device.hpp"

namespace lum::ahi::fmod {

	class FMODDevice : public AudioDevice {
	public:

		void Initialize( int32, Flags<InitFlag> ) override;
		void Finalize( ) override;

		SoundHandle LoadSound( StringView, Flags<SoundFlag> ) override;
		void UnloadSound( SoundHandle& ) override;

		AudioEffectHandle CreateEffect( const FAudioEffectDescriptor& ) override;
		AudioEffectHandle CreateEffect( ahi::EffectPreset preset ) override;
		void DeleteEffect( AudioEffectHandle& ) override;

		ChannelGroupHandle CreateChannelGroup( StringView ) override;
		void AddGroupEffect( ChannelGroupHandle, AudioEffectHandle ) override;
		void RemoveGroupEffect( ChannelGroupHandle, AudioEffectHandle ) override;

		void PlayOneShot( SoundHandle, const FPlaybackDescriptor& ) override;
		void Play( FSoundInstance&, ChannelGroupHandle ) override;

		void StopAll( ) override;

		void SetMasterVolume( float32 ) override;
		void Set3DListenerAttributes( glm::vec3, glm::vec3, glm::vec3, glm::vec3 ) override;
		void Set3DSettings( float32, float32, float32 ) override;

		void Update( FSoundInstance& ) override;


	private:

		FMOD::System* mSystem = nullptr;

		static constexpr FMOD_REVERB_PROPERTIES skReverbPresets[] =
		{
			FMOD_PRESET_OFF,
			FMOD_PRESET_GENERIC,
			FMOD_PRESET_PADDEDCELL,
			FMOD_PRESET_ROOM,
			FMOD_PRESET_BATHROOM,
			FMOD_PRESET_LIVINGROOM,
			FMOD_PRESET_STONEROOM,
			FMOD_PRESET_AUDITORIUM,
			FMOD_PRESET_CONCERTHALL,
			FMOD_PRESET_CAVE,
			FMOD_PRESET_ARENA,
			FMOD_PRESET_HANGAR,
			FMOD_PRESET_STONECORRIDOR,
			FMOD_PRESET_ALLEY,
			FMOD_PRESET_FOREST,
			FMOD_PRESET_CITY,
			FMOD_PRESET_MOUNTAINS,
			FMOD_PRESET_QUARRY,
			FMOD_PRESET_PLAIN,
			FMOD_PRESET_PARKINGLOT,
			FMOD_PRESET_SEWERPIPE,
			FMOD_PRESET_UNDERWATER
		};

		FMOD::Sound*		to_fmod_sound( vptr ptr ) const noexcept { return static_cast<FMOD::Sound*>(ptr); }
		FMOD::Channel*		to_fmod_channel( vptr ptr ) const noexcept { return static_cast<FMOD::Channel*>(ptr); }
		FMOD::ChannelGroup* to_fmod_chgroup( vptr ptr ) const noexcept { return static_cast<FMOD::ChannelGroup*>(ptr); }
		FMOD::DSP*			to_fmod_dsp( vptr ptr )	const noexcept { return static_cast<FMOD::DSP*>(ptr); }

		static FMOD_MODE translate_sound_flags( Flags<SoundFlag> );
		static FMOD_INITFLAGS translate_init_flags( Flags<InitFlag> );

		FMOD::DSP* create_reverb_effect( const FAudioEffectDescriptor::FReverb& );
		FMOD::DSP* create_frequency_effect( const FAudioEffectDescriptor::FFrequencyPass::FPass&, detail::FrequnecyType );
		FMOD::DSP* create_echo_effect( const FAudioEffectDescriptor::FEcho& );
		FMOD::DSP* create_distortion_effect( const FAudioEffectDescriptor::FDistortion& );
		FMOD::DSP* create_chorus_effect( const FAudioEffectDescriptor::FChorus& );
		FMOD::DSP* create_flange_effect( const FAudioEffectDescriptor::FFlange& );
		FMOD::DSP* create_compressor_effect( const FAudioEffectDescriptor::FCompressor& );
	

	};

}