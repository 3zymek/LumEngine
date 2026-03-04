//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Name component for the ECS.
//
//=============================================================================//
#pragma once

#include "core/utils/fixed_string.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component holding a fixed-length name string for an entity.
	*  Used primarily for debugging, editor display, and logging.
	*  Zero heap allocations — name is stored inline as a FixedString<32>.
	*/
	struct CName : public Component {

		/* @brief Null-terminated fixed-size name string. Max 31 usable characters. */
		FixedString<32> mName;

	};

} // namespace lum