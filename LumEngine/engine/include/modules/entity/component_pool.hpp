//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"

namespace lum::ecs { class MEntityManager; }

namespace lum::ecs::detail {

	/* @brief Type-erased base for all component pools.
	*
	* Allows MEntityManager to store pools of different types
	* in a single array and call Remove without knowing the component type.
	*/
	struct BasePool {
		virtual bool Contains( EntityID entityID ) = 0;
		virtual void Remove( EntityID& entityID ) = 0;
		virtual StringView GetName( ) = 0;
		virtual ~BasePool( ) { }
	};

	/* @brief Typed component pool backed by a SparseSet.
	*
	* Stores components of type tType indexed by EntityID.
	* Provides O(1) add, remove, lookup and cache-friendly iteration.
	* Each component type has its own pool managed by MEntityManager.
	*
	* @tparam tType Component type. Must satisfy the Component concept.
	*/
	template<Component tType>
	class ComponentPool : public BasePool {
	public:

		friend class MEntityManager;

		/* @brief Adds a component to the given entity.
		* If the entity already has this component, returns the existing one.
		* @param entityID Target entity ID.
		* @param component Component value to store.
		* @return Reference to the stored component.
		*/
		tType& Add( EntityID entityID, tType component ) {
			if (!Has( entityID ))
				mComponents.Append( component, entityID );
			return mComponents[ entityID ];
		}

		/* @brief Returns a pointer to the component on the given entity.
		* @param entityID Target entity ID.
		* @return Pointer to component, or nullptr if not present.
		*/
		tType* Get( EntityID entityID ) {
			return mComponents.Get( entityID );
		}

		/* @brief Removes the component from the given entity.
		* @param entityID Target entity ID.
		*/
		void Remove( EntityID& entityID ) override {
			mComponents.Remove( entityID );
			entityID = skNullEntity;
		}

		bool Contains( EntityID entityID ) override {
			return Has( entityID );
		}

		/* @brief Checks whether the given entity has this component.
		* @param entityID Target entity ID.
		* @return True if the component exists on the entity.
		*/
		bool Has( EntityID entityID ) {
			return mComponents.Contains( entityID );
		}

		StringView GetName( ) override {
			return GetComponentName<tType>( );
		}


	protected:

		cstd::SparseSet<EntityID, tType> mComponents{ limits::gMaxEntity };

	};

} // namespace lum::ecs::detail