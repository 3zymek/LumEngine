#pragma once

#include "audio_common.hpp"
#include "ahi/core/ahi_sound.hpp"

namespace lum {

	namespace ahi { 
		class AudioDevice; 
		struct FSoundInstance; 
		struct SoundHandle;
		struct ChannelGroupHandle;
	}

	namespace ecs { class EntityManager; }

}

namespace lum::audio {

	class AudioManager {
	public:
		
		AudioManager() = default;
		~AudioManager() {}
		
		void Initialize( ahi::AudioDevice* device );
		
		void LoadSound(StringView relativePath, StringView alias, SoundCategory cat);
		void UnloadSound(StringView alias);

		ahi::InstID CreateInstance(StringView alias);

		void Stop(ahi::InstID instanceID);
		void SetPause(ahi::InstID instanceID, bool paused);
		void SetVolume(ahi::InstID instanceID, float32 volume);
		void SetPitch(ahi::InstID instanceID, float32 pitch);
		void SetPosition(ahi::InstID instanceID, glm::vec3 pos);

		void Update(ecs::EntityManager* mgr);

	private:
		
		ahi::AudioDevice* mDevice = nullptr;
		
		std::unordered_map<uint64, ahi::SoundHandle>    mSounds;
		std::unordered_map<uint64, ahi::ChannelGroupHandle> mGroups;
		std::unordered_map<ahi::InstID, ahi::FSoundInstance> mInstances;

	};

}