#pragma once
#include "ecs_define.hpp"
#include <sparse_set.hpp>
#include <optional>

struct BasePool {};

namespace ecs {
	namespace detail {
		template<Component T>
		class ComponentPool : public BasePool {
		public:

			force_inline std::optional<std::reference_wrapper<T>> Add(const EntityID& _id) {
				if (m_components.contains(_id)) return std::nullopt;
				T component;
				m_components.append(component, _id);
				return std::ref(m_components[_id]);
			}
			force_inline void Remove(const EntityID& _id) {
				if (m_components.contains(_id)) m_components.remove(_id);
			}
			force_inline std::optional<std::reference_wrapper<T>> Get(const EntityID& _id) {
				if (m_components.contains(_id)) return std::ref(m_components[_id]);
				return std::nullopt;
			}

		private:

			cstd::sparse_set<T> m_components{ MAX_POOL_CAPACITY };

		};


	}
}
