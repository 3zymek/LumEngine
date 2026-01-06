#pragma once
#include "core/core_defines.hpp"
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {
    namespace ecs {

        using EntityID = uint32_t;

        namespace detail {

            inline constexpr unsigned int MAX_POOL_CAPACITY = settings::ECS_MAX_POOL_CAPACITY;
            inline constexpr unsigned int MAX_ENTITY_COUNT = settings::ECS_MAX_ENTITY_COUNT;
            inline constexpr unsigned int MAX_COMPONENT_TYPES_COUNT = settings::ECS_MAX_COMPONENT_TYPES_COUNT;

            #define LumComponentTag \
                static constexpr bool __lumcomponent__ = true;

            struct UniqueComponent {};

            template<typename T>
            concept Component =
                requires { T::__lumcomponent__ == true; }&&
                    std::is_trivially_copyable_v<T>&&
                    std::is_trivially_destructible_v<T>;

            using ComponentTID = uint32_t;

            struct ComponentTypeID {
                template<Component ComponentType>
                static ComponentTID Get() {
                    static ComponentTID id = globalID++;
                    return id;
                }
            private:

                static inline ComponentTID globalID = 0;

            };
        }
    }
}