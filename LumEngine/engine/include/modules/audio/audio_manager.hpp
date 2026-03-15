#pragma once

#include "audio_common.hpp"

namespace lum {

	namespace ahi { 
		class AudioDevice; 
		struct FSoundInstance; 
		struct SoundHandle;
		struct ChannelGroupHandle;
	}

}

namespace lum::audio {

	class AudioManager {
	public:
		
		AudioManager() = default;
		~AudioManager() {}
		
		void Initialize( ahi::AudioDevice* device );
		
		void LoadSound(StringView relativePath, StringView alias, SoundCategory cat);
		void UnloadSound(StringView alias);

		void CreateInstance(StringView alias);

		void SetVolume();
		void SetPitch();
		void SetPosition();

	private:
		
		ahi::AudioDevice* mDevice = nullptr;
		
		std::unordered_map<uint64, ahi::SoundHandle>    mSounds;
		std::unordered_map<uint64, ahi::ChannelGroupHandle> mGroups;
		std::vector<ahi::FSoundInstance> mInstances;

	};

}