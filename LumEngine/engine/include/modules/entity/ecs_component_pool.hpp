#pragma once
#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"
#include "core/core_pch.hpp"
#include "core/lum_assert.hpp"
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

				iterator begin() { return m_components.begin(); }
				iterator end() { return m_components.end(); }

				const_iterator begin() const { return m_components.begin(); }
				const_iterator end() const { return m_components.end(); }

				inline T& operator[]( usize index ) {
					return m_components[index];
				}

				inline usize size() {
					return m_components.dense_size();
				}

				T* Add( EntityID entityID ) {
					LUM_HOTPATH_ASSERT_NULLPTR(
						m_components.contains(entityID), std::format("Entity {} already have {} component", entityID, typeid(T).name())
					);
					T component;
					m_components.emplace(component, entityID);
					return &m_components[entityID];
				}

				inline void Remove( EntityID entityID ) {
					LUM_HOTPATH_ASSERT_VOID(
						!m_components.contains(entityID), std::format("Entity doesn't have {} component to remove", typeid(T).name())
					);
					m_components.remove(entityID);
				}

				inline T* Get( EntityID entityID ) {
					LUM_HOTPATH_ASSERT_NULLPTR(
						!m_components.contains(entityID), std::format("Entity doesn't have {} component", typeid(T).name())
					);
					return &m_components[entityID];
				}

				inline bool Has( EntityID entityID ) {
					return m_components.contains(entityID);
				}

			private:

				cstd::sparse_set<T, ComponentIndex> m_components{ MAX_POOL_CAPACITY };

			};
		}
	}
}