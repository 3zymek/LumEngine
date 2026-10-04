//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Buffer management configuration (VBO, EBO, UBO, SSBO)
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {
	
	struct BufferCreateInfoOLD {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		BufferUsageOLD m_BufferUsage = BufferUsageOLD::Static;

		// Defines type of buffer (VBO, EBO, UBO, SSBO)
		BufferTypeOLD m_BufferType = BufferTypeOLD::None;

		// Size of data that's assigned.
		usize m_BufferSize = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<MapFlag> m_MapFlags{};

		// Pointer to data.
		const void* m_Data = nullptr;
	};

	struct BufferOLD {

		BufferIDOLD		m_Handle = 0;

		BufferTypeOLD		m_Type = BufferTypeOLD::None;
		BufferUsageOLD		m_Usage = BufferUsageOLD::Static;
		Flags<MapFlag>	m_Flags{};
		usize			m_Size{};
		bool			m_Mapped = false;

	};

}