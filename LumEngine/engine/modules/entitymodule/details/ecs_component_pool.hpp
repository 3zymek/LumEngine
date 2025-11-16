#pragma once
#include "ecs_define.hpp"
#include "utils/sparse_set.hpp"
#include <optional>

struct BasePool {};

namespace ecs {
	namespace detail {
		template<Component T>
		class ComponentPool : public BasePool {
		public:

			force_inline T* Add(const EntityID& _id) {
				if (m_components.contains(_id)) return nullptr;
				T component;
				m_components.append(component, _id);
				return &m_components[_id];
			}
			force_inline void Remove(const EntityID& _id) {
				if (m_components.contains(_id)) m_components.remove(_id);
			}
			force_inline T* Get(const EntityID& _id) {
				if (m_components.contains(_id)) return &m_components[_id];
				return nullptr;
			}

		private:

			cstd::sparse_set<T> m_components{ MAX_POOL_CAPACITY };

		};


	}
}
