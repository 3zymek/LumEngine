#pragma once
#include "core/core_defines.hpp"
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {
    namespace ecs {

        using EntityID = uint32;

        namespace detail {

            #define LUM_COMPONENT_TAG \
                static constexpr bool __lumcomponent__ = true;

            struct UniqueComponent {};

            template<typename T>
            concept Component =
                requires { T::__lumcomponent__ == true; }&&
                    std::is_trivially_copyable_v<T>&&
                    std::is_trivially_destructible_v<T>;

            using ComponentTID = uint32;

            struct ComponentTypeID {
                template<Component ComponentType>
                static ComponentTID Get() {
                    return GenerateID<ComponentType>::Get();
                }

            };
        }
    }
}