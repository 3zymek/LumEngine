#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct VertexLayoutHandle {

		layoutid id = std::numeric_limits<layoutid>::max();
		layoutid generation = 0;

	};

	// Attributes for single vertex in vertex layout
	struct VertexAttribute {
		// Data in buffer format (Float, Float2, Mat4, etc.)
		DataFormat format{};
		// Offset where to start reading single attribute. 
		// example: offsetof(Vertex, color).
		usize relative_offset = 0;
		// Location to shader attribute.
		usize shader_location = 0;
	};
	struct VertexLayoutDescriptor {

		// Pointer to the array of vertex attributes.
		std::span<const VertexAttribute> attributes;
		// Sizeof single element in buffer.
		usize stride	= 0;
		// Binding slot (use only when you're making multiple buffers in one layout, otherwise binding = 0).
		usize binding = 0;
		// Offset where to start reading buffer (use only when you're making multiple buffers in one layout, otherwise offset = 0).
		usize offset = 0;

	};

	struct VertexLayout {
		
		layoutid vao = 0;

	};

}