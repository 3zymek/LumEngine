//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Defines audio emitter properties for an entity.
//          Consumed by AudioManager to sync state with AHI.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component storing audio emitter properties for an entity. */
	struct CAudioEmitter : public Component {

		/* @brief Playback volume. Range [0, 1]. */
		float32 mVolume = 1.0f;

		/* @brief Playback pitch multiplier. 1.0 = normal speed. */
		float32 mPitch = 1.0f;

		/* @brief World position of the emitter for 3D audio. */
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };

		/* @brief Distance at which the sound starts attenuating. */
		float32 mMinDistance = 10.0f;

		/* @brief Distance at which the sound is fully attenuated. */
		float32 mMaxDistance = 50.0f;

		/* @brief Whether playback is currently paused. */
		bool bPaused = false;

		/* @brief Whether the emitter is in an active playing state (true even if paused). */
		bool bPlaying = false;

		/* @brief Whether playback should loop. */
		bool bLooped = false;

		/* @brief Marks the emitter as modified. Set to true by default to trigger initial sync. */
		bool bMarked = true;

	};

} // namespace lum