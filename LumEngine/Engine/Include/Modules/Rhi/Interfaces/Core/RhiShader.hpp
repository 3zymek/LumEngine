#pragma once

#include "Rhi/RhiTypes.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Platform/FileSystem/Path.hpp"

namespace lum::rhi {

	enum class ShaderStage : uint8 {
		Vertex = 0U,
		Fragment,
		Compute,
		Geometry
	};

	struct ShaderInfo2 {

		Path m_Path{}; // Path to shader from project's root
		ShaderStage m_Stage{};
		String m_EntryPoint{};

	};

} // namespace lum::rhi