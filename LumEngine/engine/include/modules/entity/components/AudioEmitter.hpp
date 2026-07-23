//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines audio emitter properties for an entity.
//          Consumed by AudioManager to sync state with AHI.
//
//=============================================================================//
#pragma once
#include "Ahi/AhiCommon.hpp"
#include "Entity/EcsCommon.hpp"
#include "Entity/Components/AudioEmitter.gen.hpp"

namespace lum {

	// TODO: AUDIO IN LUMENGINE

	LCLASS( Category = "AUDIO" )
	struct CAudioEmitter : public ComponentBase {

		//LUM_GENERATED_BODY( )

		inline static StringView sCategoryName = "AUDIO";
		inline static StringView sSerializationName = "audio_emitter";
		inline static StringView sDisplayName = "Audio Emitter";

		/* @brief Handle to loaded sound */
		ahi::SoundHandle mSound;

		ahi::ChannelGroupHandle mGroup;

		/* @brief Playback volume. Range [0, 1]. */
		LPROPERTY( MinVal = 0.0, MaxVal = 1.0 ) 
		float32 mVolume = 1.0f;

		/* @brief Playback pitch multiplier. 1.0 = normal speed. */
		LPROPERTY( MinVal = 0.0, MaxVal = 1.0 ) 
		float32 mPitch = 1.0f;

		/* @brief Distance at which the sound starts attenuating. */
		LPROPERTY( )
		float32 mMinDistance = 10.0f;

		/* @brief Distance at which the sound is fully attenuated. */
		LPROPERTY( )
		float32 mMaxDistance = 50.0f;

		/* @brief Whether playback is currently paused. */
		bool mPaused = false;

		/* @brief Whether the emitter is in an active playing state (true even if paused). */
		bool mPlaying = false;
		bool mStop = false;
		bool mPlay = false;

		/* @brief Whether playback should loop. */
		bool mLooped = false;

	}; 
	
	LUM_CLASS_EXTENSIONS( )

} // namespace lum