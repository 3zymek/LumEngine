#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct BufferHandle {
		
		bufferid id = max_val<bufferid>();
		bufferid generation = 0;

	};

	struct BufferDescriptor {
		BufferUsage	buffer_usage	= BufferUsage::Static;
		size_t		size			= 0;
		mapflag	map_flags			= 0;
		cvptr		data			= nullptr;
	};

	struct Buffer {

		BufferType	type	{};
		BufferUsage usage	{};
		mapflag		flags	= 0;
		size_t		size	= 0;
		bool		mapped	= false;

		union {
			bufferid gl_handle = 0;
		} handle{};

	};

}