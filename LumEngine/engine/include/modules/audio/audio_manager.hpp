#pragma once

#include "audio_common.hpp"
#include "ahi/core/ahi_sound.hpp"
#include "entity/entity_manager.hpp"

namespace lum {

	namespace ahi {
		class AudioDevice;
		struct FSoundInstance;
		struct SoundHandle;
		struct ChannelGroupHandle;
	}

}

namespace lum::audio {

	class MAudioManager {
	public:
		
		MAudioManager() = default;
		~MAudioManager() {}

		void Initialize( ahi::AudioDevice* device );
		
		void LoadSound(StringView relativePath, StringView alias, SoundCategory cat);
		void UnloadSound(StringView alias);

		ahi::SoundHandle GetSound(StringView alias);

		void Stop(ahi::InstID instanceID);
		void SetPause(ahi::InstID instanceID, bool paused);
		void SetVolume(ahi::InstID instanceID, float32 volume);
		void SetPitch(ahi::InstID instanceID, float32 pitch);
		void SetPosition(ahi::InstID instanceID, glm::vec3 pos);

		void Update(ecs::MEntityManager* mgr);

	private:
		
		ahi::AudioDevice* mDevice = nullptr;
		
		std::unordered_map<uint64, ahi::SoundHandle>    mSounds;
		std::unordered_map<uint64, ahi::ChannelGroupHandle> mGroups;
		std::unordered_map<ecs::EntityID, ahi::FSoundInstance> mInstances;

	};

}