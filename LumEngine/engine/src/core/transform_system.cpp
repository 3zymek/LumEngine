//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Updates world space transform matrices for all entities in a scene.
//
//=============================================================================//

#include "core/transform_system.hpp"
#include "scene/scene_manager.hpp"
#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/relationship.hpp"

namespace lum {

	void TransformSystem::Update( FScene* scene ) {

		ecs::MEntityManager& mgr = scene->mEntityMgr;

		mgr.EachWithID<CTransform>(
			[&]( EntityID id, CTransform& transform ) {

				Matrix4& matrix = transform.mWorldMatrix = Matrix4( 1.0f );

				matrix = Translate( matrix, transform.mPosition );
				matrix = Rotate( matrix, transform.mRotation );
				matrix = Scale( matrix, transform.mScale );

				CChild* child = mgr.GetComponent<CChild>( id );

				if (child) {

					CTransform* parentTransform = mgr.GetComponent<CTransform>( child->mParentID );

					if (!parentTransform)
						return;

					matrix = parentTransform->mWorldMatrix * matrix;

				}

			}
		);


	}

} // namespace lum