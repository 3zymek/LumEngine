//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: High-level audio manager — sound loading, playback,
//          channel groups, effects and 3D listener management.
//
//=============================================================================//

#include "audio/audio_manager.hpp"

#include "ahi/core/ahi_device.hpp"

#include "core/utils/asset_loader.hpp"

#include "event/event_bus.hpp"

#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/audio_emitter.hpp"
#include "entity/components/camera.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MAudioManager::Initialize( ahi::AudioDevice* device, ev::EventBus* bus ) {

		mDevice = device;
		mEventBus = bus;

		mEventBus->SubscribePermanently<EEntityDestroyed>(
			[&]( const EEntityDestroyed& ev ) {

				if (mInstances.contains( ev.mID ))
					mInstances[ ev.mID ].mFlags.Enable( ahi::InstanceFlag::Kill );

			} );

	}

	ahi::SoundHandle MAudioManager::GetSound( StringView relativePath, SoundCategory cat ) {

		HashedStr hash = HashStr( relativePath );
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

		HashedStr hash = HashStr( alias );

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

	void MAudioManager::StopAll( ) {

		mDevice->StopAll( );

	}

	ahi::AudioEffectHandle MAudioManager::CreateEffect( StringView name, ahi::EffectPreset preset ) {

		return CreateEffect( name, ahi::detail::gkEffectPresetLookup[ ToUnderlyingEnum( preset ) ] );

	}
	ahi::AudioEffectHandle MAudioManager::CreateEffect( StringView name, const ahi::FAudioEffectDescriptor& desc ) {

		HashedStr hash = HashStr( name );

		if (!mEffects.contains( hash )) {
			mEffects[ hash ] = mDevice->CreateEffect( desc );
		}

		return mEffects[ hash ];

	}
	void MAudioManager::DeleteEffect( StringView name ) {

		HashedStr hash = HashStr( name );
		LUM_RETURN_IF( !mEffects.contains( hash ), LUM_SEV_WARN, "Effect named %s doesn't exist", name.data( ) );
		DeleteEffect( mEffects[ hash ] );

	}
	void MAudioManager::DeleteEffect( ahi::AudioEffectHandle effect ) {

		mDevice->DeleteEffect( effect );

	}

	ahi::AudioEffectHandle MAudioManager::GetEffect( StringView name ) {

		HashedStr hash = HashStr( name );

		if (!mEffects.contains( hash )) {
			LUM_LOG_WARN( "Effect named %s doesn't exist", name.data( ) );
			return {};
		}

		return mEffects[ hash ];

	}

	ahi::ChannelGroupHandle MAudioManager::GetGroup( StringView name ) {

		HashedStr hash = HashStr( name );

		if (!mGroups.contains( hash )) {
			mGroups[ hash ] = mDevice->CreateChannelGroup( name );
		}

		return mGroups[ hash ];

	}

	void MAudioManager::SetGroupEffect( ahi::ChannelGroupHandle group, ahi::AudioEffectHandle effect ) {

		mDevice->SetGroupEffect( group, effect );

	}
	void MAudioManager::SetGroupEffect( ahi::ChannelGroupHandle group, StringView effect ) {

		HashedStr hash = HashStr( effect );
		LUM_RETURN_IF( !mEffects.contains( hash ), LUM_SEV_WARN, "Invalid effect" );
		SetGroupEffect( group, mEffects[ hash ] );

	}
	void MAudioManager::SetGroupEffect( StringView group, ahi::AudioEffectHandle effect ) {

		HashedStr hash = HashStr( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Invalid group" );
		SetGroupEffect( mGroups[ hash ], effect );

	}
	void MAudioManager::SetGroupEffect( StringView group, StringView effect ) {

		HashedStr hashGroup = HashStr( group );
		HashedStr hashEffect = HashStr( effect );

		LUM_RETURN_IF( !mGroups.contains( hashGroup ), LUM_SEV_WARN, "Invalid group" );
		LUM_RETURN_IF( !mEffects.contains( hashEffect ), LUM_SEV_WARN, "Invalid effect" );
		SetGroupEffect( mGroups[ hashGroup ], mEffects[ hashEffect ] );

	}
	void MAudioManager::SetGroupVolume( StringView group, float32 volume ) {

		HashedStr hash = HashStr( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupVolume( mGroups[ hash ], volume );

	}
	void MAudioManager::SetGroupVolume( ahi::ChannelGroupHandle group, float32 volume ) {

		mDevice->SetGroupVolume( group, volume );

	}
	void MAudioManager::SetGroupPitch( StringView group, float32 pitch ) {

		HashedStr hash = HashStr( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupPitch( mGroups[ hash ], pitch );

	}
	void MAudioManager::SetGroupPitch( ahi::ChannelGroupHandle group, float32 pitch ) {

		mDevice->SetGroupPitch( group, pitch );

	}

	void MAudioManager::SetMasterVolume( float32 volume ) {

		mDevice->SetMasterVolume( volume );

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

		mgr->EachWithID<CTransform, CAudioEmitter>(
			[&]( EntityID id, CTransform& transf, CAudioEmitter& emitter ) {

				if (emitter.bMarked) {

					auto& inst = mInstances[ id ];

					inst.mVolume = emitter.mVolume;
					inst.mPitch = emitter.mPitch;
					inst.mMinDistance = emitter.mMinDistance;
					inst.mMaxDistance = emitter.mMaxDistance;
					if (emitter.bPaused)
						inst.mFlags.Enable( ahi::InstanceFlag::Paused );
					if (emitter.bPlaying)
						inst.mFlags.Enable( ahi::InstanceFlag::Playing );
					if (emitter.bLooped)
						inst.mFlags.Enable( ahi::InstanceFlag::Looped );
					if (emitter.bPlay)
						inst.mFlags.Enable( ahi::InstanceFlag::Play );
					inst.mSound = emitter.mSound;

					inst.mPosition = transf.mPosition;

				}

				emitter.bMarked = false;

			} );

		for (auto it = mInstances.begin( ); it != mInstances.end( );) {

			mDevice->UpdateInstance( it->second );
			if (it->second.mFlags.Has( ahi::InstanceFlag::Kill )) {
				it = mInstances.erase( it );
			}
			else ++it;


		}

	}


}