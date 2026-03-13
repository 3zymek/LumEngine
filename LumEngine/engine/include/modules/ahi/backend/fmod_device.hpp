#pragma once

#include "ahi/core/ahi_device.hpp"
#include "ahi/core/ahi_effect.hpp"

namespace lum::ahi::fmod {

	class FMODDevice : public AudioDevice {
	public:

		void Initialize() override;
		void Finalize() override;

		SoundHandle LoadSound(StringView) override;
		void UnloadSound(SoundHandle&) override;

		AudioEffectHandle CreateEffect(const FAudioEffectDescriptor&) override;

		ChannelGroupHandle CreateChannelGroup(StringView) override;

		void PlayOneShot(SoundHandle, const FPlaybackDescriptor&) override;
		void Play(const FSoundInstance&, ChannelGroupHandle) override;

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

		vptr create_reverb_effect(const FAudioEffectDescriptor::FReverb&);
	

	};

}