/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Utils/TypeID.hpp"
#include "Entity/EcsTypes.hpp"

#include "Entity/Components/Transform.gen.hpp"

namespace lum {

	/* @brief ECS component storing the 3D transform of an entity. */
    LUM_CLASS( Category = "TRANSFORM" )
    struct CTransform : public ComponentBase {

        LUM_GENERATED_BODY( )

        /* @brief Local position relative to parent. */
        LUM_PROPERTY( ) Vector3 m_Position = Vector3( 0.0f );

        /* @brief Local non-uniform scale relative to parent. */
        LUM_PROPERTY( ) Vector3 m_Scale = Vector3( 1.0f );

        /* @brief Local Euler rotation in degrees relative to parent. */
        LUM_PROPERTY( ) Vector3 m_Rotation = Vector3( 0.0f );

        /* @brief World space transform matrix, computed by TransformSystem. */
        Matrix4 m_WorldMatrix = Matrix4( 1.0f );

	};

    LUM_CLASS_EXTENSIONS( )

} // namespace lum