//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shader configuration for the Rendering Hardware Interface.
//
//=============================================================================//
#pragma once

#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	/* @brief Descriptor used to supply source code for shader compilation. */
	struct ShaderCreateInfo {
		String m_VertexContent;   /* @brief Raw source code string for the vertex shader stage. */
		String m_FragmentContent; /* @brief Raw source code string for the fragment/pixel shader stage. */
	};

	/* @brief Represents a compiled native shader program object. */
	struct Shader {

		ShaderID m_Handle = 0;    /* @brief Native graphics API handle to the compiled shader program. */

	};

} // namespace lum::rhi