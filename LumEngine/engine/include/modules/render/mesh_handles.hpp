//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Mesh resource handles.
//
//=============================================================================//
#pragma once

#include "core/utils/handle_pool.hpp"

namespace lum {

	struct StaticMeshHandle : cstd::BaseHandle<uint32> { };
	struct DynamicMeshHandle : cstd::BaseHandle<uint32> { };

} // namespace lum