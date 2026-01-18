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
		BufferUsage		buffer_usage	= BufferUsage::Static;
		size_t			size			= 0;
		mapflag_t			map_flags		= 0;
		const void*		data			= nullptr;
	};

	struct Buffer {

		BufferType	type	{};
		BufferUsage usage	{};
		mapflag_t		flags	= 0;
		size_t		size	= 0;
		bool		mapped	= false;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

}