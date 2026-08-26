//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shader configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	struct ShaderCreateInfo {
		String m_VertexContent;
		String m_FragmentContent;
	};

	struct Shader {

		ShaderID m_Handle = 0;

	};

}