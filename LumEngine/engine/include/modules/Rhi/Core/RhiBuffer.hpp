//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Buffer management configuration (VBO, EBO, UBO, SSBO)
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	struct BufferCreateInfo {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		BufferUsage m_BufferUsage = BufferUsage::Static;

		// Defines type of buffer (VBO, EBO, UBO, SSBO)
		BufferType m_BufferType = BufferType::None;

		// Size of data that's assigned.
		usize m_Size = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<MapFlag> m_MapFlags{};

		// Pointer to data.
		const void* m_Data = nullptr;
	};

	struct Buffer {

		BufferID		m_Handle = 0;

		BufferType		m_Type = BufferType::None;
		BufferUsage		m_Usage = BufferUsage::Static;
		Flags<MapFlag>	m_Flags{};
		usize			m_Size{};
		bool			m_Mapped = false;

	};

}