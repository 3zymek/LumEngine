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

	void AudioManager::Initialize( ahi::IAudioDevice* device, ev::EventBus& bus ) {

		m_Device = device;
		m_EventBus = bus;

		m_EventBus().SubscribePermanently<EEntityDestroyed>(
			[&]( const EEntityDestroyed& ev ) {

				if (m_Instances.contains( ev.m_Id ))
					m_Instances[ ev.m_Id ].m_Flags.Set( ahi::SoundInstanceFlag::Kill );

			} );

	}

	ahi::SoundHandle AudioManager::FindSound( StringView relativePath, SoundCategory cat ) {

		HashedString hash = HashString( relativePath );
		if (m_Sounds.contains( hash )) return m_Sounds[ hash ];

		Path fullPath = ResourceLoader::ResolveResourcePath( ResourceRoot::External, relativePath );

		ahi::SoundHandle handle = m_Device->LoadSound( fullPath, ahi::detail::GetSoundFlags( cat ) );
		m_Sounds.insert( { hash, handle } );

		LUM_LOG_INFO( "Loaded sound %s", relativePath.data( ) );
		return handle;

	}
	ahi::SoundHandle AudioManager::FindSound( StringView relativePath, StringView cat ) {

		return FindSound( relativePath, ahi::detail::ParseSoundCategory( cat ) );

	}
	void AudioManager::UnloadSound( StringView alias ) {

		HashedString hash = HashString( alias );

		if (m_Sounds.contains( hash ))
			m_Sounds.erase( HashString( alias ) );

	}

	void AudioManager::PlayOneShot( StringView relativePath, const ahi::SoundPlaybackDescription& desc ) {

		ahi::SoundHandle sound = FindSound( relativePath );
		m_Device->PlayOneShot( sound, desc );

	}

	void AudioManager::Set3DListenerAttributes( const ahi::ListenerAttributes& attrs ) {

		m_Device->Set3DListenerAttributes( attrs );

	}

	void AudioManager::StopAll( ) {

		m_Device->StopAll( );

	}

	ahi::AudioEffectHandle AudioManager::CreateEffect( StringView name, ahi::EffectPreset preset ) {

		return CreateEffect( name, ahi::detail::k_EffectPresetLookup[ ToUnderlyingEnum( preset ) ] );

	}
	ahi::AudioEffectHandle AudioManager::CreateEffect( StringView name, const ahi::AudioEffectCreateInfo& desc ) {

		HashedString hash = HashString( name );

		if (!m_Effects.contains( hash )) {
			m_Effects[ hash ] = m_Device->CreateEffect( desc );
		}

		return m_Effects[ hash ];

	}
	void AudioManager::DeleteEffect( StringView name ) {

		HashedString hash = HashString( name );
		LUM_RETURN_IF( !m_Effects.contains( hash ), LUM_SEV_WARN, "Effect named %s doesn't exist", name.data( ) );
		DeleteEffect( m_Effects[ hash ] );

	}
	void AudioManager::DeleteEffect( ahi::AudioEffectHandle effect ) {

		m_Device->DeleteEffect( effect );

	}

	ahi::AudioEffectHandle AudioManager::FindEffect( StringView name ) {

		HashedString hash = HashString( name );

		if (!m_Effects.contains( hash )) {
			LUM_LOG_WARN( "Effect named %s doesn't exist", name.data( ) );
			return {};
		}

		return m_Effects[ hash ];

	}

	ahi::ChannelGroupHandle AudioManager::FindGroup( StringView name ) {

		HashedString hash = HashString( name );

		if (!m_Groups.contains( hash )) {
			m_Groups[ hash ] = m_Device->CreateChannelGroup( name );
		}

		return m_Groups[ hash ];

	}

	void AudioManager::BindEffectToGroup( ahi::ChannelGroupHandle group, ahi::AudioEffectHandle effect ) {

		m_Device->SetGroupEffect( group, effect );

	}
	void AudioManager::BindEffectToGroup( ahi::ChannelGroupHandle group, StringView effect ) {

		HashedString hash = HashString( effect );
		LUM_RETURN_IF( !m_Effects.contains( hash ), LUM_SEV_WARN, "Invalid effect" );
		BindEffectToGroup( group, m_Effects[ hash ] );

	}
	void AudioManager::BindEffectToGroup( StringView group, ahi::AudioEffectHandle effect ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !m_Groups.contains( hash ), LUM_SEV_WARN, "Invalid group" );
		BindEffectToGroup( m_Groups[ hash ], effect );

	}
	void AudioManager::BindEffectToGroup( StringView group, StringView effect ) {

		HashedString hashGroup = HashString( group );
		HashedString hashEffect = HashString( effect );

		LUM_RETURN_IF( !m_Groups.contains( hashGroup ), LUM_SEV_WARN, "Invalid group" );
		LUM_RETURN_IF( !m_Effects.contains( hashEffect ), LUM_SEV_WARN, "Invalid effect" );
		BindEffectToGroup( m_Groups[ hashGroup ], m_Effects[ hashEffect ] );

	}
	void AudioManager::SetGroupVolume( StringView group, float32 volume ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !m_Groups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupVolume( m_Groups[ hash ], volume );

	}
	void AudioManager::SetGroupVolume( ahi::ChannelGroupHandle group, float32 volume ) {

		m_Device->SetGroupVolume( group, volume );

	}
	void AudioManager::SetGroupPitch( StringView group, float32 pitch ) {

		HashedString hash = HashString( group );
		LUM_RETURN_IF( !m_Groups.contains( hash ), LUM_SEV_WARN, "Group %s dosen't exist", group.data( ) );
		SetGroupPitch( m_Groups[ hash ], pitch );

	}
	void AudioManager::SetGroupPitch( ahi::ChannelGroupHandle group, float32 pitch ) {

		m_Device->SetGroupPitch( group, pitch );

	}

	void AudioManager::SetMasterVolume( float32 volume ) {

		m_Device->SetMasterVolume( volume );

	}

	void AudioManager::UpdateInstances( ecs::EntityManager* mgr ) {

		mgr->Each<CCamera, CTransform>(
			[&]( CCamera& camera, CTransform& transform ) {
				ahi::ListenerAttributes attrs;

				attrs.m_Position = transform.m_Position;
				attrs.m_Up = camera.m_Up;
				attrs.m_Forward = Normalize( camera.m_Target - transform.m_Position );

				Set3DListenerAttributes( attrs );

			} );

		mgr->EachWithID<CTransform, CAudioEmitter>(
			[&]( EntityID id, CTransform& transf, CAudioEmitter& emitter ) {

				/*
				if (emitter.m_Marked) {

					auto& inst = m_Instances[ id ];

					inst.m_Volume = emitter.m_Volume;
					inst.m_Pitch = emitter.m_Pitch;
					inst.m_MinDistance = emitter.m_MinDistance;
					inst.m_MaxDistance = emitter.m_MaxDistance;
					if (emitter.m_Paused)
						inst.m_Flags.Set( ahi::SoundInstanceFlag::Paused );
					if (emitter.m_Playing)
						inst.m_Flags.Set( ahi::SoundInstanceFlag::Playing );
					if (emitter.m_Looped)
						inst.m_Flags.Set( ahi::SoundInstanceFlag::Looped );
					if (emitter.m_Play)
						inst.m_Flags.Set( ahi::SoundInstanceFlag::Play );
					inst.m_Sound = emitter.m_Sound;

					inst.m_Position = transf.m_Position;

				}

				emitter.m_Marked = false;
				*/

			} );

		for (auto it = m_Instances.begin( ); it != m_Instances.end( );) {

			m_Device->UpdateInstance( it->second );
			if (it->second.m_Flags.Has( ahi::SoundInstanceFlag::Kill )) {
				it = m_Instances.erase( it );
			}
			else ++it;


		}

	}


}