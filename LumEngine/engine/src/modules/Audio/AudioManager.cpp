//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: High-level audio manager — sound loading, playback,
//          channel groups, effects and 3D listener management.
//
//=============================================================================//

#include "Audio/AudioManager.hpp"

#include "Ahi/Core/AhiDevice.hpp"

#include "Core/Utils/ResourceLoader.hpp"

#include "Event/EventBus.hpp"

#include "Entity/EntityManager.hpp"
#include "Entity/Components/Transform.hpp"
#include "Entity/Components/AudioEmitter.hpp"
#include "Entity/Components/Camera.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void AudioManager::Initialize( ahi::AudioDevice* device, ev::EventBus* bus ) {

		mDevice = device;
		mEventBus = bus;

		mEventBus->SubscribePermanently<EEntityDestroyed>(
			[&]( const EEntityDestroyed& ev ) {

				if (mInstances.contains( ev.mId ))
					mInstances[ ev.mId ].mFlags.Set( ahi::SoundInstanceFlag::Kill );

			} );

	}

	ahi::SoundHandle AudioManager::FindSound( StringView relativePath, SoundCategory cat ) {

		HashedString hash = HashString( relativePath );
		if (mSounds.contains( hash )) return mSounds[ hash ];

		String fullPath = ResourceLoader::ResolvePath( ResourceRoot::External, relativePath );
		if (fullPath.empty( )) {
			LUM_LOG_ERROR( "Couldn't load audio file: %s", ResourceLoader::GetErrorMessage( ) );
			return {};
		}

		ahi::SoundHandle handle = mDevice->LoadSound( fullPath, ahi::detail::GetSoundFlags( cat ) );
		mSounds.insert( { hash, handle } );

		LUM_LOG_INFO( "Loaded sound %s", relativePath.data( ) );
		return handle;

	}
	ahi::SoundHandle AudioManager::FindSound( StringView relativePath, StringView cat ) {

		return FindSound( relativePath, ahi::detail::ParseSoundCategory( cat ) );

	}
	void AudioManager::UnloadSound( StringView alias ) {

		HashedString hash = HashString( alias );

		if (mSounds.contains( hash ))
			mSounds.erase( HashString( alias ) );

	}

	void AudioManager::PlayOneShot( StringView relativePath, const ahi::SoundPlaybackDescription& desc ) {

		ahi::SoundHandle sound = FindSound( relativePath );
		mDevice->PlayOneShot( sound, desc );

	}

	void AudioManager::Set3DListenerAttributes( const ahi::ListenerAttributes& attrs ) {

		mDevice->Set3DListenerAttributes( attrs );

	}

	void AudioManager::StopAll( ) {

		mDevice->StopAll( );

	}

	ahi::AudioEffectHandle AudioManager::CreateEffect( StringView name, ahi::EffectPreset preset ) {

		return CreateEffect( name, ahi::detail::kEffectPresetLookup[ ToUnderlyingEnum( preset ) ] );

	}
	ahi::AudioEffectHandle AudioManager::CreateEffect( StringView name, const ahi::AudioEffectCreateInfo& desc ) {

		HashedString hash = HashString( name );

		if (!mEffects.contains( hash )) {
			mEffects[ hash ] = mDevice->CreateEffect( desc );
		}

		return mEffects[ hash ];

	}
	void AudioManager::DeleteEffect( StringView name ) {

		HashedString hash = HashString( name );
		LUM_RETURN_IF( !mEffects.contains( hash ), LUM_SEV_WARN, "Effect named %s doesn't exist", name.data( ) );
		DeleteEffect( mEffects[ hash ] );

	}
	void AudioManager::DeleteEffect( ahi::AudioEffectHandle effect ) {

		mDevice->DeleteEffect( effect );

	}

	ahi::AudioEffectHandle AudioManager::FindEffect( StringView name ) {

		HashedString hash = HashString( name );

		if (!mEffects.contains( hash )) {
			LUM_LOG_WARN( "Effect named %s doesn't exist", name.data( ) );
			return {};
		}

		return mEffects[ hash ];

	}

	ahi::ChannelGroupHandle AudioManager::FindGroup( StringView name ) {

		HashedString hash = HashString( name );

		if (!mGroups.contains( hash )) {
			mGroups[ hash ] = mDevice->CreateChannelGroup( name );
		}

		return mGroups[ hash ];

	}

	void AudioManager::BindEffectToGroup( ahi::ChannelGroupHandle group, ahi::AudioEffectHandle effect ) {

		mDevice->SetGroupEffect( group, effect );

	}
	void AudioManager::BindEffectToGroup( ahi::ChannelGroupHandle group, StringView effect ) {

		HashedString hash = HashString( effect );
		LUM_RETURN_IF( !mEffects.contains( hash ), LUM_SEV_WARN, "Invalid effect" );
		BindEffectToGroup( group, mEffects[ hash ] );

	}
	void AudioManager::BindEffectToGroup( StringView group, ahi::AudioEffectHandle effect ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Invalid group" );
		BindEffectToGroup( mGroups[ hash ], effect );

	}
	void AudioManager::BindEffectToGroup( StringView group, StringView effect ) {

		HashedString hashGroup = HashString( group );
		HashedString hashEffect = HashString( effect );

		LUM_RETURN_IF( !mGroups.contains( hashGroup ), LUM_SEV_WARN, "Invalid group" );
		LUM_RETURN_IF( !mEffects.contains( hashEffect ), LUM_SEV_WARN, "Invalid effect" );
		BindEffectToGroup( mGroups[ hashGroup ], mEffects[ hashEffect ] );

	}
	void AudioManager::SetGroupVolume( StringView group, float32 volume ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupVolume( mGroups[ hash ], volume );

	}
	void AudioManager::SetGroupVolume( ahi::ChannelGroupHandle group, float32 volume ) {

		mDevice->SetGroupVolume( group, volume );

	}
	void AudioManager::SetGroupPitch( StringView group, float32 pitch ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !mGroups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupPitch( mGroups[ hash ], pitch );

	}
	void AudioManager::SetGroupPitch( ahi::ChannelGroupHandle group, float32 pitch ) {

		mDevice->SetGroupPitch( group, pitch );

	}

	void AudioManager::SetMasterVolume( float32 volume ) {

		mDevice->SetMasterVolume( volume );

	}

	void AudioManager::UpdateInstances( ecs::EntityManager* mgr ) {

		mgr->Each<CCamera, CTransform>(
			[&]( CCamera& camera, CTransform& transform ) {
				ahi::ListenerAttributes attrs;

				attrs.mPosition = transform.mPosition;
				attrs.mUp = camera.mUp;
				attrs.mForward = Normalize( camera.mTarget - transform.mPosition );

				Set3DListenerAttributes( attrs );

			} );

		mgr->EachWithID<CTransform, CAudioEmitter>(
			[&]( EntityID id, CTransform& transf, CAudioEmitter& emitter ) {

				if (emitter.mMarked) {

					auto& inst = mInstances[ id ];

					inst.mVolume = emitter.mVolume;
					inst.mPitch = emitter.mPitch;
					inst.mMinDistance = emitter.mMinDistance;
					inst.mMaxDistance = emitter.mMaxDistance;
					if (emitter.mPaused)
						inst.mFlags.Set( ahi::SoundInstanceFlag::Paused );
					if (emitter.mPlaying)
						inst.mFlags.Set( ahi::SoundInstanceFlag::Playing );
					if (emitter.mLooped)
						inst.mFlags.Set( ahi::SoundInstanceFlag::Looped );
					if (emitter.mPlay)
						inst.mFlags.Set( ahi::SoundInstanceFlag::Play );
					inst.mSound = emitter.mSound;

					inst.mPosition = transf.mPosition;

				}

				emitter.mMarked = false;

			} );

		for (auto it = mInstances.begin( ); it != mInstances.end( );) {

			mDevice->UpdateInstance( it->second );
			if (it->second.mFlags.Has( ahi::SoundInstanceFlag::Kill )) {
				it = mInstances.erase( it );
			}
			else ++it;


		}

	}


}