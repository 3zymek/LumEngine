//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Sets engine hard limits
//
//=============================================================================//
#pragma once
#include "core/types.hpp"
#include "render/shaders_define.h"

/* @brief Contains compile-time constants that define hard upper limits
*  for all engine subsystems. Adjust these to tune memory reservation.
*/
namespace lum::limits {

	//============================= ECS ==================================

	/* @brief Maximum number of component instances per component pool. */
	inline constexpr usize kMaxEcsPoolCapacity = 1024;

	/* @brief Maximum number of entities that can exist simultaneously. */
	inline constexpr usize kMaxEntity = 1024;

	/* @brief Maximum number of distinct component types registered in the engine. */
	inline constexpr usize kMaxComponentTypes = 128;

	//============================= Audio ==================================

	/* @brief Maximum number of audio sound assets that can be loaded at once. */
	inline constexpr usize kMaxAudioSounds = 256;

	/* @brief Maximum number of concurrent audio channels. */
	inline constexpr usize kMaxAudioChannels = 256;

	/* @brief Maximum number of active audio instances playing simultaneously. */
	inline constexpr usize kMaxAudioInstances = 64;

	/* @brief Maximum number of audio channel groups for mixing and routing. */
	inline constexpr usize kMaxChannelsGroup = 8;

	/* @brief Maximum number of audio effects applied to a single channel or group. */
	inline constexpr usize kMaxAudioEffects = 10;

	//============================= Events ==================================

	/* @brief Maximum number of distinct event types registered in the engine. */
	inline constexpr usize kMaxEventTypes = 128;

	/* @brief Maximum number of callbacks invoked per frame for a single event type. */
	inline constexpr usize kMaxCallbackPerFrame = 8;

	/* @brief Maximum number of persistent callbacks registered for a single event type. */
	inline constexpr usize kMaxPermanentCallbacks = 8;

	/* @brief Maximum number of times a single event type can be emitted per frame. */
	inline constexpr usize kMaxEventEmitsPerFrame = 64;

	//============================= Render ==================================

	/* @brief Maximum number of models that can be loaded simultaneously. */
	inline constexpr usize kMaxModels = 256;

	/* @brief Maximum number of lights active in a single frame. Mirrors LUM_MAX_LIGHTS. */
	inline constexpr usize kMaxLights = LUM_MAX_LIGHTS;

	/* @brief Maximum number of materials that can be loaded simultaneously. */
	inline constexpr usize kMaxMaterials = 256;

	/* @brief Maximum number of textures that can be loaded simultaneously. */
	inline constexpr usize kMaxTextures = 256;

	/* @brief Maximum number of draw calls per frame. */
	inline constexpr usize kMaxDrawCallsPerFrame = 256;

	/* @brief Maximum number of spot lights that can cast shadows simultaneously. */
	inline constexpr uint8 kMaxShadowCastingSpotLights = LUM_MAX_SHADOW_CASTING_SPOT_LIGHTS;


	//============================= Assets ==================================

	/* @brief Maximum character length of an error message generated during asset loading. */
	inline constexpr usize kMaxErrorAssetLoadLength = 128;

} // namespace lum::limits