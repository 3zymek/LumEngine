#pragma once
#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"
#include "core/core_pch.hpp"

struct BasePool {};

namespace ecs {
	namespace detail {
		template<Component T>
		class ComponentPool : public BasePool {
		public:

			force_inline T& operator[](size_t index) {
				return m_components[index];
			}

			force_inline size_t size() {
				return m_components.dense_size();
			}

			force_inline T* Add(EntityID entityID) {
				if (m_components.contains(entityID)) return nullptr;
				T component;
				m_components.emplace(component, entityID);
				return &m_components[entityID];
			}

			force_inline void Remove(EntityID entityID) {
				if (m_components.contains(entityID)) m_components.remove(entityID);
			}

			force_inline T* Get(EntityID entityID) {
				if (m_components.contains(entityID)) return &m_components[entityID];
				return nullptr;
			}

			force_inline bool Has(EntityID entityID) {
				return m_components.contains(entityID);
			}

		private:

			cstd::sparse_set<T> m_components{ MAX_POOL_CAPACITY };

		};


	}
}
