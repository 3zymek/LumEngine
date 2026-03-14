#pragma once

#include "ahi/core/ahi_device.hpp"
#include "ahi/core/ahi_effect.hpp"

namespace lum::ahi::fmod {

	class FMODDevice : public AudioDevice {
	public:

		void Initialize() override;
		void Finalize() override;

		SoundHandle LoadSound(StringView, Flags<SoundFlag>) override;
		void UnloadSound(SoundHandle&) override;

		AudioEffectHandle CreateEffect(const FAudioEffectDescriptor&) override;

		ChannelGroupHandle CreateChannelGroup(StringView) override;
		void AddEffectToGroup(ChannelGroupHandle, AudioEffectHandle) override;

		void PlayOneShot(SoundHandle, const FPlaybackDescriptor&) override;
		void Play(FSoundInstance&, ChannelGroupHandle) override;

		void StopAll() override;

		void SetMasterVolume(float32) override;
		void AddEffectToSound(SoundHandle, AudioEffectHandle) override;
		void Set3DListenerPosition(glm::vec3, glm::vec3 forward, glm::vec3 up) override;

		void Update(std::vector<FSoundInstance>& instances) override;


	private:

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

		FMOD::System* mSystem = nullptr;

		static FMOD_MODE translate_sound_flags(Flags<SoundFlag>);

		FMOD::DSP* create_reverb_effect(const FAudioEffectDescriptor::FReverb&);
		FMOD::DSP* create_frequency_effect(const FAudioEffectDescriptor::FFrequencyPass::FPass&, detail::FrequnecyType type);
		FMOD::DSP* create_echo_effect(const FAudioEffectDescriptor::FEcho&);
		FMOD::DSP* create_distortion_effect(const FAudioEffectDescriptor::FDistortion&);
		FMOD::DSP* create_chorus_effect(const FAudioEffectDescriptor::FChorus&);
		FMOD::DSP* create_flange_effect(const FAudioEffectDescriptor::FFlange&);
		FMOD::DSP* create_compressor_effect(const FAudioEffectDescriptor::FCompressor&);
	

	};

}