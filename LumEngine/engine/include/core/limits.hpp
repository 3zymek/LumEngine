//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Sets engine hard limits
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/shaders_define.h"

/* @brief Contains compile-time constants that define hard upper limits
*  for all engine subsystems. Adjust these to tune memory reservation.
*/
namespace lum::limits {

	//============================= ECS ==================================

	/* @brief Maximum number of component instances per component pool. */
	LUM_COMPILE_VARIABLE usize gMaxEcsPoolCapacity = 1024;

	/* @brief Maximum number of entities that can exist simultaneously. */
	LUM_COMPILE_VARIABLE usize gMaxEntity = 1024;

	/* @brief Maximum number of distinct component types registered in the engine. */
	LUM_COMPILE_VARIABLE usize gMaxComponentTypes = 32;

	//============================= Audio ==================================

	/* @brief Maximum number of audio sound assets that can be loaded at once. */
	LUM_COMPILE_VARIABLE usize gMaxAudioSounds = 256;

	/* @brief Maximum number of concurrent audio channels. */
	LUM_COMPILE_VARIABLE usize gMaxAudioChannels = 256;

	//============================= Events ==================================

	/* @brief Maximum number of distinct event types registered in the engine. */
	LUM_COMPILE_VARIABLE usize gMaxEventTypes = 32;

	/* @brief Maximum number of callbacks invoked per frame for a single event type. */
	LUM_COMPILE_VARIABLE usize gMaxCallbackPerFrame = 8;

	/* @brief Maximum number of persistent callbacks registered for a single event type. */
	LUM_COMPILE_VARIABLE usize gMaxPermanentCallbacks = 8;

	/* @brief Maximum number of times a single event type can be emitted per frame. */
	LUM_COMPILE_VARIABLE usize gMaxEventEmittsPerFrame = 64;

	//============================= Render ==================================

	/* @brief Maximum number of models that can be loaded simultaneously. */
	LUM_COMPILE_VARIABLE usize gMaxModels = 256;

	/* @brief Maximum number of lights active in a single frame. Mirrors LUM_MAX_LIGHTS. */
	LUM_COMPILE_VARIABLE usize gMaxLights = LUM_MAX_LIGHTS;

	/* @brief Maximum number of materials that can be loaded simultaneously. */
	LUM_COMPILE_VARIABLE usize gMaxMaterials = 256;

	/* @brief Maximum number of textures that can be loaded simultaneously. */
	LUM_COMPILE_VARIABLE usize gMaxTextures = 256;

	//============================= Assets ==================================

	/* @brief Maximum character length of an error message generated during asset loading. */
	LUM_COMPILE_VARIABLE usize gMaxErrorAssetLoadLength = 128;

} // namespace lum::limits