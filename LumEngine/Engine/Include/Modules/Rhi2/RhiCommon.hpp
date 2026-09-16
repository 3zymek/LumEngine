#pragma once

#include "Rhi/RhiPch.hpp"
#include "Core/CoreCommon.hpp"
#include "Core/Utils/Logger.hpp"

namespace lum::rhi {

#	define LUM_MAX_FRAMES_IN_FLIGHT 2

	struct Vertex {
		Vector3 m_Position = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 m_Color = Vector3( 0.0f, 0.0f, 0.0f );
		Vector2 m_Uv = Vector2( 0.0f, 0.0f );
	};

} // namespace lum::rhi