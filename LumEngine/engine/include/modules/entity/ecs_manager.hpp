#pragma once

#include "core/limits.hpp"
#include "entity/ecs_common.hpp"
#include "entity/ecs_component_pool.hpp"

namespace lum {

	struct Entity;

	namespace ecs {

		class MEntityManager {
		public:
			
			template<detail::Component tType>
			tType AddComponent(EntityID entityID) {

			}
			template<detail::Component tType>
			tType AddComponent(const Entity& entity) {

			}
			template<detail::Component tType>
			tType RemoveComponent(EntityID entityID) {

			}
			template<detail::Component tType>
			tType RemoveComponent(const Entity& entity) {

			}

			template<detail::Component tType>
			bool Has(EntityID entityID) {

			}
			template<detail::Component tType>
			bool Has(const Entity& entity) {

			}
			
			
		private:

			void init() {

			}
			void destroy() {

			}

			detail::BaseComponent* mComponentPools [ limits::gMaxComponents ];

		};


	}

}