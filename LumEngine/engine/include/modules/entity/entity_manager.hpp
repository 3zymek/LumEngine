//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once
#include "core/limits.hpp"

#include "entity/ecs_common.hpp"
#include "entity/component_pool.hpp"

namespace lum {

	class Entity;
	class ManagedEntity;

	namespace ev { class EventBus; }

	namespace ecs {

		/* @brief Manages entity component pools and their lifecycle.
		*
		* Central manager for the ECS system. Handles creation and destruction
		* of component pools, adding/removing components per entity, and
		* iterating over entities that share a set of component types.
		*/
		class MEntityManager {
		public:

			MEntityManager( ) { init( ); }
			MEntityManager( MEntityManager&& other ) noexcept {
				for (int32 i = 0; i < limits::gMaxComponentTypes; i++) {
					mComponentPools[ i ] = other.mComponentPools[ i ];
					other.mComponentPools[ i ] = nullptr;
				}
			}
			~MEntityManager( ) { destroy( ); }


			void Initialize( ev::EventBus* bus ) { mEventBus = bus; }


			/* @brief Creates a new entity and returns a managed handle to it.
			*  @return ManagedEntity wrapping the newly created entity ID.
			*/
			ManagedEntity CreateEntity( );

			/* @brief Adds a component of type tType to the given entity.
			* @param entityID Target entity ID.
			* @param component Component to create (default constructor by default)
			* @return Reference to the newly added component.
			*/
			template<detail::Component tType>
			tType& AddComponent( EntityID entityID, tType component = {} );

			/* @brief Adds a component of type tType to the given entity.
			* @param entity Target entity.
			* @param component Component to create (default constructor by default)
			* @return Reference to the newly added component.
			*/
			template<detail::Component tType>
			tType& AddComponent( Entity entity, tType component = {} );





			/* @brief Returns a pointer to the component of type tType on the given entity.
			* @param entityID Target entity ID.
			* @return Pointer to component, or nullptr if not present.
			*/
			template<detail::Component tType>
			tType* GetComponent( EntityID entityID );

			/* @brief Returns a pointer to the component of type tType on the given entity.
			* @param entity Target entity.
			* @return Pointer to component, or nullptr if not present.
			*/
			template<detail::Component tType>
			tType* GetComponent( Entity entity );





			/* @brief Removes a component of type tType from the given entity.
			* @param entityID Target entity ID.
			*/
			template<detail::Component tType>
			void RemoveComponent( EntityID entityID );

			/* @brief Removes a component of type tType from the given entity.
			* @param entity Target entity.
			*/
			template<detail::Component tType>
			void RemoveComponent( Entity entity );





			/* @brief Checks whether the given entity has a component of type tType.
			* @param entityID Target entity ID.
			* @return True if the component exists on the entity.
			*/
			template<detail::Component tType>
			bool HasComponent( EntityID entityID );

			/* @brief Checks whether the given entity has a component of type tType.
			* @param entity Target entity.
			* @return True if the component exists on the entity.
			*/
			template<detail::Component tType>
			bool HasComponent( Entity entity );

			/* @brief Checks whether the given entity has a components of types.
			* @param entityID Target entity ID.
			* @return True if the components exists on the entity.
			*/
			template<ecs::detail::Component tType, ecs::detail::Component... tRest>
			bool HasComponents( EntityID entityID );

			/* @brief Checks whether the given entity has a components of types.
			* @param entity Target entity.
			* @return True if the components exists on the entity.
			*/
			template<ecs::detail::Component tType, ecs::detail::Component... tRest>
			bool HasComponents( Entity entity );





			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entityID Target entity ID.
			*/
			void DestroyEntity( EntityID& entityID );

			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entity Target entity.
			*/
			void DestroyEntity( Entity& entity );





			/* @brief Iterates over all entities that have tFirst and all tRest components.
			* Callback receives component references without entity ID.
			* @param callback Callable with signature (tFirst&, tRest&...).
			*/
			template<detail::Component tFirst, detail::Component... tRest, typename tCallback>
			void Each( tCallback&& callback );

			/* @brief Iterates over all entities that have tFirst and all tRest components.
			* Callback receives entity ID followed by component references.
			* @param callback Callable with signature (EntityID, tFirst&, tRest&...).
			*/
			template<detail::Component tFirst, detail::Component... tRest, typename tCallback>
			void EachWithID( tCallback&& callback );

			/* @brief Iterates over all components that have entity has.
			* Callback receives base pool and makes it available for usage.
			* @param callback Callable with signature [ ]( ecs::detail::ComponentPool* pool ) { ... }.
			*/
			template<typename tCallback>
			void ForEachComponent( EntityID id, tCallback&& callback ) {
				for (int32 i = 0; i < limits::gMaxComponentTypes; i++) {
					if (mComponentPools[ i ] && mComponentPools[ i ]->Contains( id )) {
						callback( mComponentPools[ i ] );
					}
				}
			}


			/* @brief Returns the component pool for type tType, creating it if it doesn't exist.
			* @return Reference to the ComponentPool for tType.
			*/
			template<detail::Component tType>
			detail::ComponentPool<tType>& GetPool( );


		private:

			/* @brief Array of component pools indexed by component type ID. */
			detail::BasePool* mComponentPools[ limits::gMaxComponentTypes ];

			ev::EventBus* mEventBus = nullptr;

			/* @brief Initializes all pool slots to nullptr. */
			void init( );

			/* @brief Deletes all allocated component pools. */
			void destroy( );

		};


	}

}
#include "entity/entity_manager.ipp"