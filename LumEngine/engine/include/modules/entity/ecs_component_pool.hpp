#pragma once
#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"
#include "core/core_pch.hpp"
#include "core/utils/lum_assert.hpp"
namespace lum {
	namespace ecs {
		namespace detail {

			struct BasePool {};

			template<Component T>
			class ComponentPool : public BasePool {

				using ComponentIndex	= usize;
				using iterator			= cstd::sparse_set<T, ComponentIndex>::iterator;
				using const_iterator	= cstd::sparse_set<T, ComponentIndex>::const_iterator;

			public:

				iterator begin() { return mComponents.begin(); }
				iterator end() { return mComponents.end(); }

				const_iterator begin() const { return mComponents.begin(); }
				const_iterator end() const { return mComponents.end(); }

				inline T& operator[]( usize index ) {
					return mComponents[index];
				}

				inline usize size() {
					return mComponents.dense_size();
				}

				T* Add( EntityID entityID ) {
					LUM_HOTCHK_RETURN_NPTR(
						mComponents.contains(entityID), LUM_SEV_DEBUG, "Entity {} already have {} component"
					);
					T component;
					mComponents.emplace(component, entityID);
					return &mComponents[entityID];
				}

				inline void Remove( EntityID entityID ) {
					LUM_HOTCHK_RETURN_VOID(
						!mComponents.contains(entityID), LUM_SEV_DEBUG, "Entity doesn't have {} component to remove"
					);
					mComponents.remove(entityID);
				}

				inline T* Get( EntityID entityID ) {
					LUM_HOTCHK_RETURN_NPTR(
						!mComponents.contains(entityID), LUM_SEV_DEBUG, "Entity doesn't have {} component"
					);
					return &mComponents[entityID];
				}

				inline bool Has( EntityID entityID ) {
					return mComponents.contains(entityID);
				}

			private:

				cstd::sparse_set<T, ComponentIndex> mComponents{ limits::gMaxEcsPoolCapacity };

			};
		}
	}
}