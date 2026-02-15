//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shader configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"

namespace lum::rhi {

	struct ShaderDescriptor {
		ccharptr vertexSource;
		ccharptr fragmentSource;
	};

	struct Shader {

		ShaderID handle = 0;

	};

}