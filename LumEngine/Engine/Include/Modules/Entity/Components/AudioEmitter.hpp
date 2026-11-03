/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once
#include "Ahi/AhiTypes.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/TypeID.hpp"
#include "Entity/EcsTypes.hpp"

#include "Entity/Components/AudioEmitter.gen.hpp"

namespace lum {

	// TODO: AUDIO IN LUMENGINE

	LUM_CLASS( Category = "AUDIO" )
	struct CAudioEmitter : public ComponentBase {

		LUM_GENERATED_BODY( )

		/* @brief Handle to loaded sound */
		ahi::SoundHandle m_Sound;

		ahi::ChannelGroupHandle m_Group;

		/* @brief Playback volume. Range [0, 1]. */
		LUM_PROPERTY( MinVal = 0.0, MaxVal = 1.0 ) 
		float32 m_Volume = 1.0f;

		/* @brief Playback pitch multiplier. 1.0 = normal speed. */
		LUM_PROPERTY( MinVal = 0.0, MaxVal = 1.0 ) 
		float32 m_Pitch = 1.0f;

		/* @brief Distance at which the sound starts attenuating. */
		LUM_PROPERTY( )
		float32 m_MinDistance = 10.0f;

		/* @brief Distance at which the sound is fully attenuated. */
		LUM_PROPERTY( )
		float32 m_MaxDistance = 50.0f;

		/* @brief Whether playback is currently paused. */
		bool m_Paused = false;

		/* @brief Whether the emitter is in an active playing state (true even if paused). */
		bool m_Playing = false;
		bool m_Stop = false;
		bool m_Play = false;

		/* @brief Whether playback should loop. */
		bool m_Looped = false;

	}; 
	
	LUM_CLASS_EXTENSIONS( )

} // namespace lum