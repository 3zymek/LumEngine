//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a single scene.
// 
//=============================================================================//

#include "Scene/Scene.hpp"

namespace lum {

	void SceneInstance::AttachChild( EntityID parent, EntityID child ) {

		if (parent == child) return;
		if (m_Parents.contains( child )) return;
		EntityID current = parent;
		while (m_Parents.contains( current )) {
			current = m_Parents[ current ];
			if (current == child) return;
		}
		m_Parents[ child ] = parent;
		m_Children[ parent ].push_back( child );

	}

	void SceneInstance::DetachChild( EntityID child ) {

		if (!m_Parents.contains( child )) return;

		EntityID parent = m_Parents[ child ];
		m_Parents.erase( child );

		auto& children = m_Children[ parent ];
		children.erase(
			std::remove( children.begin( ), children.end( ), child ), children.end( )
		);

		if (children.empty( ))
			m_Children.erase( parent );

	}

	Optional<Entity> SceneInstance::GetEntity( EntityID entity ) {

		auto it = m_Entities.find( entity );
		return (it != m_Entities.end( )) ? it->second : Optional<Entity>::Empty( );

	}

	Entity& SceneInstance::CreateEntity( ) {

		EntityID id = UniqueID<Entity>::Get( );
		return m_Entities.emplace( id, Entity( id ) ).first->second;

	}

	void SceneInstance::DeleteEntity( EntityID entity ) {

		auto it = m_Entities.find( entity );
		if (it != m_Entities.end( )) {
			for (EntityID child : m_Children[ entity ]) {
				DeleteEntity( child );
			}
			m_Children.erase( entity );
		}

		DetachChild( entity );
		m_Entities.erase( entity );
		m_EntityMgr.DestroyEntity( entity );

	}

} // namespace lum