//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a single scene.
// 
//=============================================================================//

#include "Scene/Scene.hpp"

namespace lum {

	void Scene::AttachChild( EntityID parent, EntityID child ) {

		if (parent == child) return;
		if (mParents.contains( child )) return;
		EntityID current = parent;
		while (mParents.contains( current )) {
			current = mParents[ current ];
			if (current == child) return;
		}
		mParents[ child ] = parent;
		mChildren[ parent ].push_back( child );

	}

	void Scene::DetachChild( EntityID child ) {

		if (!mParents.contains( child )) return;

		EntityID parent = mParents[ child ];
		mParents.erase( child );

		auto& children = mChildren[ parent ];
		children.erase(
			std::remove( children.begin( ), children.end( ), child ), children.end( )
		);

		if (children.empty( ))
			mChildren.erase( parent );

	}

	Optional<Entity> Scene::GetEntity( EntityID entity ) {

		auto it = mEntities.find( entity );
		return (it != mEntities.end( )) ? it->second : Optional<Entity>::Empty( );

	}

	Entity& Scene::CreateEntity( ) {

		EntityID id = UniqueID<Entity>::Get( );
		return mEntities.emplace( id, Entity( id ) ).first->second;

	}

	void Scene::DeleteEntity( EntityID entity ) {

		auto it = mEntities.find( entity );
		if (it != mEntities.end( )) {
			for (EntityID child : mChildren[ entity ]) {
				DeleteEntity( child );
			}
			mChildren.erase( entity );
		}

		DetachChild( entity );
		mEntities.erase( entity );
		mEntityMgr.DestroyEntity( entity );

	}

} // namespace lum