//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Sound instance and sound asset descriptors for AHI.
//          SoundInstance carries runtime playback state for a single
//          tracked sound, updated each frame via AudioDevice::UpdateInstance().
//
//=============================================================================//
#pragma once
#include "Ahi/AhiCommon.hpp"

namespace lum::ahi {

	using SoundInstanceID = uint32;

	enum class SoundInstanceFlag {

		// State
		Paused,
		Playing,
		Looped,

		// Actions
		Stop,
		Play,
		Kill

	};

	/* @brief Runtime state for a single tracked sound instance.
	*
	* Owns playback parameters (volume, pitch, position, loop) and a dirty flag
	* that signals AudioDevice::Update() to flush changes to the backend.
	*/
	struct SoundInstance {

		SoundInstance( ) : m_InstanceId( UniqueID<SoundInstance, SoundInstanceID>::Get() ) { }

		SoundInstanceID		m_InstanceId = 0;
		SoundHandle			m_Sound;
		ChannelGroupHandle	m_Group;
		
		float32		m_Volume = 1.0f;
		float32		m_Pitch = 1.0f;
		Vector3		m_Position;
		float32		m_MinDistance = 10.0f;
		float32		m_MaxDistance = 50.0f;

		Flags<SoundInstanceFlag> m_Flags;

	};

	/* @brief Lightweight handle to a loaded sound asset. */
	struct SoundAsset {

		uint32 m_Id = 0;

	};

} // namespace lum::ahi