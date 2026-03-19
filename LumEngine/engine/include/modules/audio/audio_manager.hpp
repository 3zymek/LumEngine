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

namespace lum {

	class MAudioManager {
	public:

		MAudioManager( ) = default;
		~MAudioManager( ) { }

		void Initialize( ahi::AudioDevice* device );

		ahi::SoundHandle GetSound( StringView relativePath, SoundCategory cat = SoundCategory::Music );
		ahi::SoundHandle GetSound( StringView relativePath, StringView cat );
		void UnloadSound( StringView relativePath );

		void PlayOneShot( StringView relativePath, const ahi::FPlaybackDescriptor& desc );

		void Set3DListenerAttributes( const ahi::FListenerAttributes& );

		void Update( ecs::MEntityManager* mgr );

	private:

		ahi::AudioDevice* mDevice = nullptr;

		std::unordered_map<uint64, ahi::SoundHandle>    mSounds;
		std::unordered_map<uint64, ahi::ChannelGroupHandle> mGroups;
		std::unordered_map<ecs::EntityID, ahi::FSoundInstance> mInstances;

	};

}