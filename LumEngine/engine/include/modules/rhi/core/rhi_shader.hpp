//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shader configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct ShaderModuleCreateInfo {
		String mVertexContent;
		String mFragmentContent;
	};

	struct Shader {

		ShaderID mHandle = 0;

	};

}