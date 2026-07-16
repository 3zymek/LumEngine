//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Updates world space transform matrices for all entities in a scene.
//
//=============================================================================//

#include "Core/TransformSystem.hpp"
#include "Scene/SceneManager.hpp"
#include "Entity/EntityManager.hpp"
#include "Entity/Components/Transform.hpp"

namespace lum {

	void TransformSystem::Update( Scene* scene ) {

		ecs::EntityManager& mgr = scene->mEntityMgr;

		mgr.EachWithID<CTransform>(
			[&]( EntityID id, CTransform& transform ) {

				if (scene->mParents.contains( id )) return;

				transform.mWorldMatrix = Matrix4( 1.0f );
				transform.mWorldMatrix = Translate( transform.mWorldMatrix, transform.mPosition );
				transform.mWorldMatrix = Rotate( transform.mWorldMatrix, transform.mRotation );
				transform.mWorldMatrix = Scale( transform.mWorldMatrix, transform.mScale );

			}
		);

		for (auto& [parent, children] : scene->mChildren) {
			for (EntityID child : children)
				update_entity_recursive( scene, child );
		}


	}

	void TransformSystem::update_entity_recursive( Scene* scene, EntityID id ) {

		ecs::EntityManager& mgr = scene->mEntityMgr;

		CTransform* transform = mgr.GetComponent<CTransform>( id );

		transform->mWorldMatrix = Matrix4( 1.0f );
		transform->mWorldMatrix = Translate( transform->mWorldMatrix, transform->mPosition );
		transform->mWorldMatrix = Rotate( transform->mWorldMatrix, transform->mRotation );
		transform->mWorldMatrix = Scale( transform->mWorldMatrix, transform->mScale );

		if (CTransform* parentTransform = mgr.GetComponent<CTransform>( scene->mParents[ id ] ))
			transform->mWorldMatrix = parentTransform->mWorldMatrix * transform->mWorldMatrix;

		if (scene->mChildren.contains( id )) {
			for (EntityID child : scene->mChildren[ id ])
				update_entity_recursive( scene, child );
		}


	}

} // namespace lum