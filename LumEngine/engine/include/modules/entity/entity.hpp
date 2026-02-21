//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"
#include "core/core_defines.hpp"

namespace lum {

	namespace ecs { class MEntityManager; }

	/* @brief Represents a unique entity in the ECS world.
	*
	* Stores only a unique ID generated at construction.
	* Can be used as a lightweight handle to identify entities
	* without access to the ECS manager.
	*/
	class Entity {
	public:
		
		friend class ecs::MEntityManager;
		Entity( ) : mID( GenerateID<Entity, 0>::Get() ) {}

		const ecs::EntityID mID;

	};

	/* @brief Extended entity with component management capabilities.
	*
	* Wraps Entity with direct access to component operations via MEntityManager.
	* Provides a fluent interface for adding, removing, and querying components.
	* Created exclusively through MEntityManager::CreateEntity().
	*
	* @note Requires a valid MEntityManager reference for the lifetime of this object.
	*/
	class ManagedEntity : public Entity {
	public:

		friend class ecs::MEntityManager;
		ManagedEntity( ecs::MEntityManager& mgr ) : mEntityManager( mgr ) {}

		template<ecs::detail::Component tType>
		tType& AddComponent( tType component = {} );

		template<ecs::detail::Component tType>
		tType* GetComponent( );

		template<ecs::detail::Component tType>
		ManagedEntity& RemoveComponent( );

		template<ecs::detail::Component tType>
		bool HasComponent( tType component = {} );

		
	protected:

		ecs::MEntityManager& mEntityManager;

	};

}
#include "entity/entity.ipp"
