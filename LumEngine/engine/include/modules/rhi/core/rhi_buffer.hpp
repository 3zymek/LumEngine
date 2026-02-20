//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Buffer management configuration (VBO, EBO, UBO, SSBO)
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct RBufferDescriptor {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		RBufferUsage mBufferUsage = RBufferUsage::Static;

		// Defines type of buffer (VBO, EBO, UBO, SSBO)
		RBufferType mBufferType = RBufferType::None;

		// Size of data that's assigned.
		usize mSize = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<RMapFlag> mMapFlags{};

		// Pointer to data.
		cvptr mData = nullptr;
	};

	struct RBuffer {

		RBufferType		mType	{};
		RBufferUsage	mUsage	{};
		Flags<RMapFlag>	mFlags	{};
		usize			mSize	= 0;
		bool			bMapped	= false;

		union {
			RBufferID gl = 0;
		} mHandle{};

	};

}