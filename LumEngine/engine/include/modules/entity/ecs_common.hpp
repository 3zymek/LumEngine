#pragma once
#include "core/core_defines.hpp"
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {
    namespace ecs {

        using EntityID = uint32;

#       define LUM_COMPONENT_TAG \
            inline constexpr static bool __lum_component__ = true;

        namespace detail {

            struct CUnique {};
            
            template<typename tType>
            concept Component =
                requires
                    { tType::__lum_component__; } &&
                    std::is_trivially_copyable_v<tType> &&
                    std::is_trivially_destructible_v<tType>; 
         
        }
    }
}