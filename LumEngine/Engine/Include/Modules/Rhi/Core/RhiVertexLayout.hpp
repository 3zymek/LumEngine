//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Vertex layout configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	// Attributes for single vertex in vertex layout
	struct VertexAttribute {

		// Data in buffer format (Float, Float2, Mat4, etc.)
		DataFormat m_Format{};

		// Offset where to start reading single attribute. 
		// example: offsetof(Vertex, color).
		usize m_RelativeOffset = 0;

		// Location to shader attribute.
		usize m_ShaderLocation = 0;
		
	};
	struct VertexLayoutCreateInfo {

		// Pointer to the array of vertex attributes.
		std::span<const VertexAttribute> m_Attributes;

		// Sizeof single element in buffer.
		usize m_Stride = 0;

		// Binding slot (use only when you're making multiple buffers in one layout, otherwise binding = 0).
		usize m_Binding = 0;

		// Offset where to start reading buffer (use only when you're making multiple buffers in one layout, otherwise offset = 0).
		usize m_Offset = 0;


	};

	struct VertexLayout {
		
		LayoutID		m_Handle = 0;
		BufferHandle	m_ElementBuff;

	};

}