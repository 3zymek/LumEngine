#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	using ShaderID = uint32;
	struct ShaderProgramHandle : public cstd::BaseHandle<ShaderID> {};

	enum class ShaderStage : uint8 {
		Vertex = 0U,
		Fragment,
		Compute,
		Geometry,

		__count
	};

	struct ShaderInfo2 {

		Path m_Path{}; // Path to shader from project's root
		ShaderStage m_Stage{};
		String m_EntryPoint{};

	};

	struct ShaderProgramCreateInfo {

		std::vector<ShaderInfo2> m_Shaders{};

	};

} // namespace lum::rhi