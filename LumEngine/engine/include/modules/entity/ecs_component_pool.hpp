#pragma once
#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace ecs {
		namespace detail {

			struct BasePool {};

			template<Component T>
			class ComponentPool : public BasePool {

				using ComponentIndex = size_t;

			public:

				inline T& operator[](size_t index) {
					return m_components[index];
				}

				inline size_t size() {
					return m_components.dense_size();
				}

				T* Add(EntityID entityID) {
					if (m_components.contains(entityID)) return nullptr;
					T component;
					m_components.emplace(component, entityID);
					return &m_components[entityID];
				}

				inline void Remove(EntityID entityID) {
					if (m_components.contains(entityID)) m_components.remove(entityID);
				}

				inline T* Get(EntityID entityID) {
					if (m_components.contains(entityID)) return &m_components[entityID];
					return nullptr;
				}

				inline bool Has(EntityID entityID) {
					return m_components.contains(entityID);
				}

			private:

				cstd::sparse_set<T, ComponentIndex> m_components{ MAX_POOL_CAPACITY };

			};
		}
	}
}