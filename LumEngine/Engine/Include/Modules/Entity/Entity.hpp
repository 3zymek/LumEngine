//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//
//=============================================================================//
#pragma once

#include "Entity/EcsCommon.hpp"

namespace lum {

	namespace ecs { class EntityManager; }

	/* @brief Represents a unique entity in the ECS world.
	*
	* Stores only a unique ID generated at construction.
	* Can be used as a lightweight handle to identify entities
	* without access to the ECS manager.
	*/
	class LUM_API Entity {
	public:

		friend class ecs::EntityManager;
		Entity( EntityID id ) : m_Id( id ) { }

		EntityID GetId( ) const noexcept { return m_Id; }
		bool IsVisible( ) const noexcept { return m_Visible; }
		void SetVisible( bool visible ) { m_Visible = visible; }

	private:

		EntityID m_Id {};
		bool m_Visible = true;

	};

	/* @brief Extended entity with component management capabilities.
	*
	* Wraps Entity with direct access to component operations via MEntityManager.
	* Provides a fluent interface for adding, removing, and querying components.
	* Created exclusively through MEntityManager::CreateEntity().
	*
	* @note Requires a valid MEntityManager reference for the lifetime of this object.
	*/
	class LUM_API ManagedEntity : public Entity {
	public:

		friend class ecs::EntityManager;

		/* @brief Constructs a managed entity bound to the given entity manager.
		* @param m_Gr Reference to the entity manager that owns this entity.
		*/
		ManagedEntity( ecs::EntityManager* m_Gr ) : Entity( UniqueID<Entity>::Get( ) ), m_EntityManager( m_Gr ) { }

		/* @brief Adds a component of type tType to this entity.
		* @param component Component value to add (default constructed if not provided).
		* @return Reference to the added component.
		*/
		template<ecs::detail::cComponent tType>
		tType& AddComponent( tType component = {} );

		/* @brief Returns a pointer to the component of type tType on this entity.
		* @return Pointer to component, or nullptr if not present.
		*/
		template<ecs::detail::cComponent tType>
		tType* GetComponent( );

		/* @brief Removes the component of type tType from this entity.
		* @return Reference to this entity for method chaining.
		*/
		template<ecs::detail::cComponent tType>
		ManagedEntity& RemoveComponent( );

		/* @brief Checks whether this entity has a component of type tType.
		* @return True if the component exists on this entity.
		*/
		template<ecs::detail::cComponent tType>
		bool HasComponent( );

		/* @brief Checks whether this entity has all specified component types.
		* @return True if all components exist on this entity.
		*/
		template<ecs::detail::cComponent tType, ecs::detail::cComponent... tRest>
		bool HasComponents( );


	protected:

		SafePtr<ecs::EntityManager> m_EntityManager = nullptr;

	};

}
#include "Entity/Entity.ipp"
