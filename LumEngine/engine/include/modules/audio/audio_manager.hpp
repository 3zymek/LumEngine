#pragma once

#include "audio_common.hpp"
#include "ahi/core/ahi_device.hpp"
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
		void StopAll( );

		ahi::AudioEffectHandle CreateEffect( StringView name, ahi::EffectPreset preset );
		ahi::AudioEffectHandle CreateEffect( StringView name, const ahi::FAudioEffectDescriptor& desc );

		ahi::AudioEffectHandle GetEffect( StringView name );

		ahi::ChannelGroupHandle GetGroup( StringView name );
		void SetGroupEffect( );

		void Update( ecs::MEntityManager* mgr );

	private:

		ahi::AudioDevice* mDevice = nullptr;

		std::unordered_map<HashedStr, ahi::SoundHandle>		mSounds;
		std::unordered_map<HashedStr, ahi::ChannelGroupHandle> mGroups;
		std::unordered_map<HashedStr, ahi::AudioEffectHandle> mEffects;
		std::unordered_map<EntityID, ahi::FSoundInstance>	mInstances;

	};

}