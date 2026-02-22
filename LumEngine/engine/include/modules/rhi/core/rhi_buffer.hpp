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

	struct FBufferDescriptor {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		EBufferUsage mBufferUsage = EBufferUsage::Static;

		// Defines type of buffer (VBO, EBO, UBO, SSBO)
		EBufferType mBufferType = EBufferType::None;

		// Size of data that's assigned.
		usize mSize = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<EMapFlag> mMapFlags{};

		// Pointer to data.
		cvptr mData = nullptr;
	};

	struct FBuffer {

		RBufferID		mHandle = 0;

		EBufferType		mType	= EBufferType::None;
		EBufferUsage	mUsage	= EBufferUsage::Static;
		Flags<EMapFlag>	mFlags	{};
		usize			mSize	= 0;
		bool			bMapped	= false;

	};

}