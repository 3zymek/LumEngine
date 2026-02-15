//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Vertex layout configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	// Attributes for single vertex in vertex layout
	struct VertexAttribute {
		// Data in buffer format (Float, Float2, Mat4, etc.)
		DataFormat mFormat{};
		// Offset where to start reading single attribute. 
		// example: offsetof(Vertex, color).
		usize mRelativeOffset = 0;
		// Location to shader attribute.
		usize mShaderLocation = 0;
		
	};
	struct VertexLayoutDescriptor {

		// Pointer to the array of vertex attributes.
		std::span<const VertexAttribute> mAttributes;
		// Sizeof single element in buffer.
		usize mStride = 0;
		// Binding slot (use only when you're making multiple buffers in one layout, otherwise binding = 0).
		usize mBinding = 0;
		// Offset where to start reading buffer (use only when you're making multiple buffers in one layout, otherwise offset = 0).
		usize mOffset = 0;

	};

	struct VertexLayout {
		
		LayoutID mHandle = 0;

	};

}