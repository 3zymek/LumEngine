//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Buffer configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct BufferDescriptor {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		BufferUsage	mBufferUsage = BufferUsage::Static;

		// Size of data that's assigned.
		usize mSize = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<Mapflag> mMapFlags{};

		// Pointer to data.
		cvptr mData = nullptr;
	};

	struct Buffer {

		BufferType		mType	{};
		BufferUsage		mUsage	{};
		Flags<Mapflag>	mFlags	{};
		usize			mSize	= 0;
		bool			bMapped	= false;

		union {
			BufferID gl = 0;
		} mHandle{};

	};

}