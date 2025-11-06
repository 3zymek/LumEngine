#pragma once
#include "e_define.hpp"

namespace ecs {

	namespace detail {

		class BaseComponent;

		using EntityID = uint32_t;
		using EntityTypeID = uint32_t;

		struct EntityTypeIndex {
			template<class EntityType>
			static EntityTypeID get() {
				static EntityTypeID id = count()++;
				return id;
			}
		private:
			static EntityTypeID& count() {
				static EntityTypeID globalID = 0;
				return globalID;
			}
		};

	#define ECS_ASSERT(expr, msg) \
		do { if(!(expr)) throw std::runtime_error(msg); } while(0);

	#define ECS_ASSERT_IS_COMPONENT(T) \
		static_assert(std::is_base_of_v<ecs::detail::BaseComponent, T> and !std::is_same_v<ecs::detail::BaseComponent, T>, "Component must be derived from BaseComponent");

	#define ECS_ASSERT_IS_SYSTEM(T) \

	}


}