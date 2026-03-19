#include "audio/audio_manager.hpp"
#include "ahi/core/ahi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/audio_emitter.hpp"
#include "entity/components/camera.hpp"

namespace lum {

	void MAudioManager::Initialize( ahi::AudioDevice* device ) {

		mDevice = device;

	}

	ahi::SoundHandle MAudioManager::GetSound( StringView relativePath, SoundCategory cat ) {

		uint64 hash = HashStr( relativePath );
		if (mSounds.contains( hash )) return mSounds[ hash ];

		String fullPath = AssetLoader::ResolvePath( RootID::External, relativePath );
		if (fullPath.empty( )) {
			LUM_LOG_ERROR( "Couldn't load audio file: %s", AssetLoader::GetErrorMessage( ) );
			return {};
		}

		ahi::SoundHandle handle = mDevice->LoadSound( fullPath, GetSoundFlags( cat ) );
		mSounds.insert( { hash, handle } );

		LUM_LOG_INFO( "Loaded sound %s", relativePath.data( ) );
		return handle;

	}
	ahi::SoundHandle MAudioManager::GetSound( StringView relativePath, StringView cat ) {

		return GetSound( relativePath, detail::ParseSoundCategory( cat ) );

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

	void MAudioManager::Set3DListenerAttributes( const ahi::FListenerAttributes& attrs ) {

		mDevice->Set3DListenerAttributes( attrs );

	}

	void MAudioManager::Update( ecs::MEntityManager* mgr ) {

		mgr->Each<CCamera, CTransform>(
			[&]( CCamera& camera, CTransform& transform ) {
				ahi::FListenerAttributes attrs;

				attrs.mPosition = transform.mPosition;
				attrs.mUp = camera.mUp;
				attrs.mForward = glm::normalize( camera.mTarget - transform.mPosition );
				
				Set3DListenerAttributes( attrs );

		} );

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

				mDevice->UpdateInstance( inst );

			}

			emitter.bMarked = false;

		} 
		);

	}


}