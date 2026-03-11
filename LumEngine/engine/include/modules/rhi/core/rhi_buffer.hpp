//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Buffer management configuration (VBO, EBO, UBO, SSBO)
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct FBufferDescriptor {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		BufferUsage mBufferUsage = BufferUsage::Static;

		// Defines type of buffer (VBO, EBO, UBO, SSBO)
		BufferType mBufferType = BufferType::None;

		// Size of data that's assigned.
		usize mSize = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<MapFlag> mMapFlags{};

		// Pointer to data.
		cvptr mData = nullptr;
	};

	struct FBuffer {

		RBufferID		mHandle = 0;

		BufferType		mType	= BufferType::None;
		BufferUsage		mUsage	= BufferUsage::Static;
		Flags<MapFlag>	mFlags	{};
		usize			mSize	= 0;
		bool			bMapped	= false;

	};

}