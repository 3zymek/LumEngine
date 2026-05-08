//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Updates world space transform matrices for all entities in a scene.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

	struct Scene;

	/* @brief Computes world space transform matrices for all entities.
	 * Must be called every frame before rendering.
	 */
	class TransformSystem {
	public:

		/* @brief Updates the world matrix of every entity with a CTransform component.
		* Propagates parent transforms down the hierarchy using CChild relationships.
		*/
		static void Update( Scene* scene );

	private:

		static void update_recursive( Scene* scene, EntityID id );

	};

} // namespace lum