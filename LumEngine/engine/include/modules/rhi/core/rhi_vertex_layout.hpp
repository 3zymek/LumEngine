#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct VertexLayoutHandle {

		uint64_t id = std::numeric_limits<uint64_t>::max();
		uint32_t generation = 0;

	};

	struct VertexAttribute {
		VertexLayoutFormat format = 0;
		size_t relative_offset = 0;
		size_t offset = 0;
		size_t shader_location = 0;
		size_t binding = 0;
	};
	struct VertexLayoutDescriptor {

		std::span<const VertexAttribute> attributes;
		size_t stride	= 0;

	};

	struct VertexLayout {
		
		uint32_t vao = 0;

	};

}