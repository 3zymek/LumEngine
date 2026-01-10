#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct BufferHandle {
		
		uint64_t id = max_val<uint64_t>();
		uint32_t generation = 0;

	};

	struct BufferDescriptor {
		size_t			size = 0;
		BufferType		buffer_type = BufferType::Vertex;
		BufferUsage		buffer_usage = BufferUsage::Static;
		MapFlag			map_flags = 0;
	};

	struct Buffer {

		BufferType	type	{};
		BufferUsage usage	{};
		MapFlag		flags	= 0;
		size_t		size	= 0;
		bool		mapped	= false;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

}