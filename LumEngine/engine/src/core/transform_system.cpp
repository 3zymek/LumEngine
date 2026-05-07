//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Updates world space transform matrices for all entities in a scene.
//
//=============================================================================//

#include "core/transform_system.hpp"
#include "scene/scene_manager.hpp"
#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"

namespace lum {

	void TransformSystem::Update( FScene* scene ) {

		ecs::MEntityManager& mgr = scene->mEntityMgr;

		mgr.EachWithID<CTransform>(
			[&]( EntityID id, CTransform& transform ) {

				Matrix4& matrix = transform.mWorldMatrix = Matrix4( 1.0f );

				matrix = Translate( matrix, transform.mPosition );
				matrix = Rotate( matrix, transform.mRotation );
				matrix = Scale( matrix, transform.mScale );

				auto it = scene->mParents.find( id );

				if (it != scene->mParents.end()) {

					EntityID parent = it->second;
					
					if( CTransform* parentTransform = mgr.GetComponent<CTransform>(parent))
						matrix = parentTransform->mWorldMatrix * matrix;

				}

			}
		);


	}

} // namespace lum