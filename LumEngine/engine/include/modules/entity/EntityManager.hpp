//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once
#include "Core/Limits.hpp"

#include "Entity/EcsCommon.hpp"
#include "Entity/ComponentPool.hpp"

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
		class LUM_API EntityManager {
		public:

			EntityManager( ) { initialize_pools( ); }

			/* @brief Move constructor. Transfers all component pool ownership from other. */
			EntityManager( EntityManager&& other ) noexcept {
				for (int32 i = 0; i < limits::kMaxComponentTypes; i++) {
					mComponentPools[ i ] = other.mComponentPools[ i ];
					other.mComponentPools[ i ] = nullptr;
				}
				mEventBus = other.mEventBus;
				other.mEventBus = nullptr;
			}

			~EntityManager( ) { destroy_pools( ); }

			/* @brief Binds the entity manager to an event bus for component lifecycle events. */
			void Initialize( ev::EventBus* bus ) { mEventBus = bus; }

			//=========================================================================
			// Entity creation / destruction
			//=========================================================================

			/* @brief Creates a new managed entity and returns a handle to it.
			*  @return ManagedEntity wrapping the newly created entity ID.
			*/
			ManagedEntity CreateEntity( );

			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entityId Target entity ID. Set to kNullEntity after destruction.
			*/
			void DestroyEntity( EntityID& entityId );

			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entity Target entity.
			*/
			void DestroyEntity( Entity& entity );

			//=========================================================================
			// Component add
			//=========================================================================

			/* @brief Adds a component of type tType to the given entity.
			* @param entityId  Target entity ID.
			* @param component Component value to store (default constructed if not provided).
			* @return Reference to the newly added component.
			*/
			template<detail::cComponent tType>
			tType& AddComponent( EntityID entityId, tType component = {} );

			/* @brief Adds a component of type tType to the given entity.
			* @param entity    Target entity.
			* @param component Component value to store (default constructed if not provided).
			* @return Reference to the newly added component.
			*/
			template<detail::cComponent tType>
			tType& AddComponent( Entity entity, tType component = {} );

			//=========================================================================
			// Component get
			//=========================================================================

			/* @brief Returns a pointer to the component of type tType on the given entity.
			* @param entityId Target entity ID.
			* @return Pointer to component, or nullptr if not present.
			*/
			template<detail::cComponent tType>
			tType* GetComponent( EntityID entityId );

			/* @brief Returns a pointer to the component of type tType on the given entity.
			* @param entity Target entity.
			* @return Pointer to component, or nullptr if not present.
			*/
			template<detail::cComponent tType>
			tType* GetComponent( Entity entity );

			//=========================================================================
			// Component remove
			//=========================================================================

			/* @brief Removes a component of type tType from the given entity.
			* @param entityId Target entity ID.
			*/
			template<detail::cComponent tType>
			void RemoveComponent( EntityID entityId );

			/* @brief Removes a component of type tType from the given entity.
			* @param entity Target entity.
			*/
			template<detail::cComponent tType>
			void RemoveComponent( Entity entity );

			//=========================================================================
			// Component queries
			//=========================================================================

			/* @brief Checks whether the given entity has a component of type tType.
			* @param entityId Target entity ID.
			* @return True if the component exists on the entity.
			*/
			template<detail::cComponent tType>
			bool HasComponent( EntityID entityId );

			/* @brief Checks whether the given entity has a component of type tType.
			* @param entity Target entity.
			* @return True if the component exists on the entity.
			*/
			template<detail::cComponent tType>
			bool HasComponent( Entity entity );

			/* @brief Checks whether the given entity has all specified component types.
			* @param entityId Target entity ID.
			* @return True if all components exist on the entity.
			*/
			template<detail::cComponent tType, detail::cComponent... tRest>
			bool HasComponents( EntityID entityId );

			/* @brief Checks whether the given entity has all specified component types.
			* @param entity Target entity.
			* @return True if all components exist on the entity.
			*/
			template<detail::cComponent tType, detail::cComponent... tRest>
			bool HasComponents( Entity entity );

			//=========================================================================
			// Iteration
			//=========================================================================

			/* @brief Iterates over all entities that have tFirst and all tRest components.
			* Callback receives component references without entity ID.
			* @param callback Callable with signature (tFirst&, tRest&...).
			*/
			template<detail::cComponent tFirst, detail::cComponent... tRest, typename tCallback>
			void Each( tCallback&& callback );

			/* @brief Iterates over all entities that have tFirst and all tRest components.
			* Callback receives entity ID followed by component references.
			* @param callback Callable with signature (EntityID, tFirst&, tRest&...).
			*/
			template<detail::cComponent tFirst, detail::cComponent... tRest, typename tCallback>
			void EachWithID( tCallback&& callback );

			/* @brief Iterates over all component pools that contain the given entity.
			* @param entityId Target entity ID.
			* @param callback Callable with signature (int32 typeIndex, ecs::ComponentBasePool* pool).
			*/
			template<typename tCallback>
			void ForEachComponent( EntityID entityId, tCallback&& callback ) {
				for (int32 i = 0; i < limits::kMaxComponentTypes; i++) {
					if (mComponentPools[ i ] && mComponentPools[ i ]->Contains( entityId ))
						callback( i, mComponentPools[ i ] );
				}
			}

			//=========================================================================
			// Pool access
			//=========================================================================

			/* @brief Returns the component pool for type tType, creating it if it doesn't exist.
			* @return Reference to the ComponentPool for tType.
			*/
			template<detail::cComponent tType>
			detail::ComponentPool<tType>& GetPool( );

		private:

			/* @brief Array of component pools indexed by component type ID. */
			ComponentBasePool* mComponentPools[ limits::kMaxComponentTypes ];

			ev::EventBus* mEventBus = nullptr;

			/* @brief Initializes all pool slots to nullptr. */
			void initialize_pools( );

			/* @brief Deletes all allocated component pools. */
			void destroy_pools( );

		};

	} // namespace lum::ecs

} // namespace lum

#include "Entity/EntityManager.ipp"