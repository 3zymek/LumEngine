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

#include "Core/Utils/Flags.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Platform/FileSystem/Path.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum::rhi {

	enum class ShaderStage : uint8 {
		Vertex = 0U,
		Fragment,
		Compute,
		Geometry
	};
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::ShaderStage );

	using ShaderStageFlags = Flags<ShaderStage>;

	struct ShaderInfo {

		Path m_Path{}; // Path to shader from project's root
		ShaderStage m_Stage{};
		String m_EntryPoint{};

	};

} // namespace lum::rhi