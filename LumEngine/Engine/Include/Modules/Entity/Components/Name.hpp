//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Name component for the ECS.
//
//=============================================================================//
#pragma once
#include "Core/Utils/FixedString.hpp"
#include "Entity/EcsCommon.hpp"
#include "Entity/Components/Name.gen.hpp"

namespace lum {

	/* @brief ECS component holding a fixed-length name string for an entity.
	*  Used primarily for debugging, editor display, and logging.
	*  Zero heap allocations — name is stored inline as a FixedString<32>.
	*/
	LUM_CLASS( ) struct CName : public ComponentBase {

		LUM_GENERATED_BODY( )

		/* @brief Null-terminated fixed-size name string. Max 31 usable characters. */
		LUM_PROPERTY( ) FixedString<32> m_Name;

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum