#pragma once

#include "ahi/core/ahi_device.hpp"

namespace lum::ahi::fmod {

	class FMODDevice : public AudioDevice {
	public:

		void Initialize() override;
		void Finalize() override;

		SoundHandle LoadSound(StringView path) override;
		void UnloadSound(SoundHandle& sound) override;

		ChannelGroupHandle CreateChannelGroup(StringView name) override;

		void PlayOneShot(SoundHandle sound, const FPlaybackDescriptor& desc) override;
		void Play(const FSoundInstance&) override;

		void Update(std::vector<FSoundInstance>& instances) override;


	private:

		FMOD::System* mSystem = nullptr;
	

	};

}