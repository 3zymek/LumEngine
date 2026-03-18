#include "audio/audio_manager.hpp"
#include "ahi/core/ahi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/audio_emitter.hpp"

namespace lum {

	void MAudioManager::Initialize( ahi::AudioDevice* device ) {

		mDevice = device;

	}

	ahi::SoundHandle MAudioManager::GetSound( StringView relativePath, SoundCategory cat ) {

		uint64 hash = HashStr( relativePath );
		if (mSounds.contains( hash )) return mSounds[ hash ];

		String fullPath = AssetLoader::ResolvePath( RootID::External, relativePath );
		LUM_RETURN_IF( fullPath.empty( ), LUM_SEV_ERROR, "Couldn't load audio file: %s", AssetLoader::GetErrorMessage( ) );

		ahi::SoundHandle handle = mDevice->LoadSound( fullPath, GetSoundFlags( cat ) );

		mSounds.insert( { hash, handle } );

		return handle;

	}
	void MAudioManager::UnloadSound( StringView alias ) {

		uint64 hash = HashStr( alias );

		if (mSounds.contains( hash ))
			mSounds.erase( HashStr( alias ) );


	}

	void MAudioManager::PlayOneShot( StringView relativePath, const ahi::FPlaybackDescriptor& desc ) {

		ahi::SoundHandle sound = GetSound( relativePath );

		mDevice->PlayOneShot( sound, desc );

	}

	void MAudioManager::Update( ecs::MEntityManager* mgr ) {


		mgr->EachWithID<CTransform, CAudioEmitter>( [&]( ecs::EntityID id, CTransform& transf, CAudioEmitter& emitter ) {

			if (emitter.bMarked) {

				auto& inst = mInstances[ id ];

				inst.mVolume = emitter.mVolume;
				inst.mPitch = emitter.mPitch;
				inst.mMinDistance = emitter.mMinDistance;
				inst.mMaxDistance = emitter.mMaxDistance;
				inst.bPaused = emitter.bPaused;
				inst.bPlaying = emitter.bPlaying;
				inst.bLooped = emitter.bLooped;
				inst.mSound = emitter.mSound;

				inst.mPosition = transf.mPosition;

				mDevice->Update( inst );

			}

			emitter.bMarked = false;

		} 
		);



	}


}