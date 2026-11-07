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

#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec2.hpp"

namespace lum::rhi {

	inline constexpr usize k_MaxFramesInFlight = 2;
	inline constexpr usize k_MaxVertexBufferBindCount = 5;

	struct Vertex {
		Vector3 m_Position = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 m_Color = Vector3( 0.0f, 0.0f, 0.0f );
		Vector2 m_Uv = Vector2( 0.0f, 0.0f );
	};

} // namespace lum::rhi