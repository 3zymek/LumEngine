//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

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
		Entity( ) : mID( GenerateID<Entity>::Get() ) {}

		const EntityID mID;

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
		
		/* @brief Constructs a managed entity bound to the given entity manager.
		* @param mgr Reference to the entity manager that owns this entity.
		*/
		ManagedEntity(ecs::MEntityManager* mgr) : mEntityManager(mgr) {}

		/* @brief Adds a component of type tType to this entity.
		* @param component Component value to add (default constructed if not provided).
		* @return Reference to the added component.
		*/
		template<ecs::detail::Component tType>
		tType& AddComponent(tType component = {});

		/* @brief Returns a pointer to the component of type tType on this entity.
		* @return Pointer to component, or nullptr if not present.
		*/
		template<ecs::detail::Component tType>
		tType* GetComponent();

		/* @brief Removes the component of type tType from this entity.
		* @return Reference to this entity for method chaining.
		*/
		template<ecs::detail::Component tType>
		ManagedEntity& RemoveComponent();

		/* @brief Checks whether this entity has a component of type tType.
		* @return True if the component exists on this entity.
		*/
		template<ecs::detail::Component tType>
		bool HasComponent();

		/* @brief Checks whether this entity has all specified component types.
		* @return True if all components exist on this entity.
		*/
		template<ecs::detail::Component tType, ecs::detail::Component... tRest>
		bool HasComponents();

		
	protected:

		ecs::MEntityManager* mEntityManager;

	};

}
#include "entity/entity.ipp"
