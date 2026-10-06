#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	enum class ShaderStage : uint8 {
		Vertex = 0U,
		Fragment,
		Compute,
		Geometry,

		_Count // ShaderStage enums count
	};

	struct ShaderInfo2 {

		Path m_Path{}; // Path to shader from project's root
		ShaderStage m_Stage{};
		String m_EntryPoint{};

	};

} // namespace lum::rhi