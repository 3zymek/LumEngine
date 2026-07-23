//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once
#include "Entity/EcsCommon.hpp"
#include "Core/Utils/SparseSet.hpp"

namespace lum::ecs { class EntityManager; }

namespace lum::ecs {

	/* @brief Type-erased base for all component pools.
	*
	* Allows MEntityManager to store pools of different types
	* in a single array and call Remove without knowing the component type.
	*/
	struct ComponentBasePool {
		virtual bool Contains( EntityID entityID ) = 0;      /* @brief Returns true if the entity has this component. */
		virtual void Remove( EntityID& entityID ) = 0;       /* @brief Removes the component from the entity. */
		virtual StringView GetParseName( ) = 0;              /* @brief Returns the serialization name of the component type. */
		virtual StringView GetDisplayName( ) = 0;            /* @brief Returns the editor display name of the component type. */
		virtual StringView GetCategoryName( ) = 0;           /* @brief Returns the editor category name of the component type. */
		virtual ~ComponentBasePool( ) { }
	};

	namespace detail {

		/* @brief Typed component pool backed by a SparseSet.
		*
		* Stores components of type tType indexed by EntityID.
		* Provides O(1) add, remove, lookup and cache-friendly iteration.
		* Each component type has its own pool managed by MEntityManager.
		*
		* @tparam tType Component type. Must satisfy the Component concept.
		*/
		template<cComponent tType>
		class ComponentPool : public ComponentBasePool {
		public:

			friend class EntityManager;

			/* @brief Adds a component to the given entity.
			* If the entity already has this component, returns the existing one.
			* @param entityID Target entity ID.
			* @param component Component value to store.
			* @return Reference to the stored component.
			*/
			tType& Add( EntityID entityId, tType component ) {
				if (!Has( entityId ))
					mComponents.Append( component, entityId );
				return mComponents[ entityId ];
			}

			/* @brief Returns a pointer to the component on the given entity.
			* @param entityID Target entity ID.
			* @return Pointer to component, or nullptr if not present.
			*/
			tType* Get( EntityID entityId ) {
				return mComponents.Get( entityId );
			}

			/* @brief Removes the component from the given entity.
			* @param entityID Target entity ID. Set to kNullEntity after removal.
			*/
			void Remove( EntityID& entityId ) override {
				mComponents.Remove( entityId );
				entityId = kNullEntity;
			}

			bool Contains( EntityID entityId ) override {
				return Has( entityId );
			}

			/* @brief Checks whether the given entity has this component.
			* @param entityID Target entity ID.
			* @return True if the component exists on the entity.
			*/
			bool Has( EntityID entityId ) {
				return mComponents.Contains( entityId );
			}

			StringView GetParseName( ) override { return tType::sSerializationName; }
			StringView GetDisplayName( ) override { return tType::sDisplayName; }
			StringView GetCategoryName( ) override { return tType::sCategoryName; }


		protected:

			cstd::SparseSet<EntityID, tType> mComponents{ limits::kMaxEntity };

		};

	} // namespace lum::ecs::detail

} // namespace lum::ecs