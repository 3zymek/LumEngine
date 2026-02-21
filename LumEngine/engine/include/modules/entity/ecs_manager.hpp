//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once

#include "core/limits.hpp"

#include "entity/ecs_common.hpp"
#include "entity/ecs_component_pool.hpp"

namespace lum {

	class Entity;
	class ManagedEntity;

	namespace ecs {


		/* @brief Manages entity component pools and their lifecycle.
		*
		* Central manager for the ECS system. Handles creation and destruction
		* of component pools, adding/removing components per entity, and
		* iterating over entities that share a set of component types.
		*/
		class MEntityManager {
		public:

			MEntityManager( ) { init(); }
			~MEntityManager( ) { destroy(); }

			
			ManagedEntity CreateEntity();



			/* @brief Adds a component of type tType to the given entity.
			* @param entityID Target entity ID.
			* @return Reference to the newly added component.
			*/
			template<detail::Component tType>
			tType& AddComponent( EntityID entityID, tType component = {} );

			/* @brief Adds a component of type tType to the given entity.
			* @param entity Target entity.
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
			




			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entityID Target entity ID.
			*/
			void DestroyEntity( EntityID entityID );

			/* @brief Destroys all components belonging to the given entity.
			* Iterates over all registered pools and removes the entity from each.
			* @param entity Target entity.
			*/
			void DestroyEntity( Entity entity );





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





			/* @brief Returns the component pool for type tType, creating it if it doesn't exist.
			* @return Reference to the ComponentPool for tType.
			*/
			template<detail::Component tType>
			detail::ComponentPool<tType>& GetPool( );
			
			
		private:
			
			void init( ) {
				for (int32 i = 0; i < limits::gMaxComponents; i++) {
					mComponentPools[i] = nullptr;
				}
			}
			void destroy( ) {
				for (int32 i = 0; i < limits::gMaxComponents; i++) {
					delete mComponentPools[i];
				}
			}

			detail::BaseComponent* mComponentPools [ limits::gMaxComponents ]; // Array of component pools indexed by type ID.
			void* mUniqueComponents[ limits::gMaxComponents ];

		};


	}

}
#include "entity/ecs_manager.ipp"