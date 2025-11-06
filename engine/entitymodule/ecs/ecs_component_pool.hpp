#pragma once
#include <unordered_map>
#include "ecs_define.hpp"
#include <optional>
namespace ecs {

	namespace detail {

		ABSTRACT_CLASS BasePool {};

		template<class Comp_t>
		class ComponentPool : public BasePool {
		public:
			
			FORCEINLINE void Add(EntityID _id) {
				m_components.emplace_back();
				m_owners.push_back(_id);
			}
			FORCEINLINE void Delete(EntityID _id) {
				for (size_t i = 0; i < m_components.size(); i++) {
					if (m_owners[i] == _id) {
						std::swap(m_owners[i], m_owners.back());
						m_owners.pop_back();
						std::swap(m_components[i], m_components.back());
						m_components.pop_back();
						return;
					}
				}
			}
			FORCEINLINE std::optional<std::reference_wrapper<Comp_t>> Get(EntityID _id) {
				for (size_t i = 0; i < m_components.size(); i++) {
					if (m_owners[i] == _id) {
						return std::ref(m_components[i]);
					}
				}
				return std::nullopt;
			}

		private:

			std::vector<Comp_t> m_components;
			std::vector<EntityID> m_owners;

		};

	}

}