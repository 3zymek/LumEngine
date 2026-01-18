#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct VertexLayoutHandle {

		uint64_t id = std::numeric_limits<uint64_t>::max();
		uint32_t generation = 0;

	};

	// Attributes for single vertex in vertex layout
	struct VertexAttribute {
		// Data in buffer format (Float, Float2, Mat4, etc.)
		layoutformat_t format = 0;
		// Offset where to start reading single attribute. 
		// example: offsetof(Vertex, color).
		size_t relative_offset = 0;
		// Location to shader attribute.
		size_t shader_location = 0;
	};
	struct VertexLayoutDescriptor {

		// Pointer to the array of vertex attributes.
		std::span<const VertexAttribute> attributes;
		// Sizeof single element in buffer.
		size_t stride	= 0;
		// Binding slot (use only when you're making multiple buffers in one layout, otherwise binding = 0).
		size_t binding = 0;
		// Offset where to start reading buffer (use only when you're making multiple buffers in one layout, otherwise offset = 0).
		size_t offset = 0;

	};

	struct VertexLayout {
		
		uint32_t vao = 0;

	};

}