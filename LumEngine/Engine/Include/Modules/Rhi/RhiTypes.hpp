#pragma once

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