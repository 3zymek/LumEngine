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
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/FixedString.hpp"
#include "Core/Utils/TypeID.hpp"
#include "Entity/EcsTypes.hpp"

#include "Entity/Components/Name.gen.hpp"

namespace lum {

	/* @brief ECS component holding a fixed-length name string for an entity.
	*  Used primarily for debugging, editor display, and logging.
	*  Zero heap allocations — name is stored inline as a FixedString<32>.
	*/
	LUM_CLASS( ) struct CName : public ComponentBase {

		LUM_GENERATED_BODY( )

		/* @brief Null-terminated fixed-size name string. Max 31 usable characters. */
		LUM_PROPERTY( ) 
		FixedString<32> m_Name;

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum