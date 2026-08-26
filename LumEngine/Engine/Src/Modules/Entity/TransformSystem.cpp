//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Updates world space transform matrices for all entities in a scene.
//
//=============================================================================//

#include "Entity/TransformSystem.hpp"
#include "Scene/SceneManager.hpp"
#include "Entity/EntityManager.hpp"
#include "Entity/Components/Transform.hpp"

namespace lum {

	void TransformSystem::Update( SceneInstance& scene ) {


		ecs::EntityManager& mgr = scene.m_EntityMgr;

		mgr.EachWithID<CTransform>(
			[ & ]( EntityID id, CTransform& transform ) {

				if (scene.m_Parents.contains( id )) return;

				transform.m_WorldMatrix = Matrix4( 1.0f );
				transform.m_WorldMatrix = Translate( transform.m_WorldMatrix, transform.m_Position );
				transform.m_WorldMatrix = Rotate( transform.m_WorldMatrix, transform.m_Rotation );
				transform.m_WorldMatrix = Scale( transform.m_WorldMatrix, transform.m_Scale );

			}
		);

		for (auto& [parent, children] : scene.m_Children) {
			for (EntityID child : children)
				update_entity_recursive( scene, child );
		}


	}

	void TransformSystem::update_entity_recursive( SceneInstance& scene, EntityID id ) {

		ecs::EntityManager& mgr = scene.m_EntityMgr;

		CTransform* transform = mgr.GetComponent<CTransform>( id );

		transform->m_WorldMatrix = Matrix4( 1.0f );
		transform->m_WorldMatrix = Translate( transform->m_WorldMatrix, transform->m_Position );
		transform->m_WorldMatrix = Rotate( transform->m_WorldMatrix, transform->m_Rotation );
		transform->m_WorldMatrix = Scale( transform->m_WorldMatrix, transform->m_Scale );

		if (CTransform* parentTransform = mgr.GetComponent<CTransform>( scene.m_Parents[ id ] ))
			transform->m_WorldMatrix = parentTransform->m_WorldMatrix * transform->m_WorldMatrix;

		if (scene.m_Children.contains( id )) {
			for (EntityID child : scene.m_Children[ id ])
				update_entity_recursive( scene, child );
		}


	}

} // namespace lum