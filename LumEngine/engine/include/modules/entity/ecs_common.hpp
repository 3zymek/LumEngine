//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"

namespace lum {
    namespace ecs {

        using EntityID = uint64;

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