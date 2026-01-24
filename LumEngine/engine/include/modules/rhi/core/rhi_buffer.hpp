#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct alignas(8) BufferHandle {
		
		bufferid id = max_val<bufferid>();
		bufferid generation = 0;

	};

	struct BufferDescriptor {
		BufferUsage	buffer_usage	= BufferUsage::static_usage;
		size_t		size			= 0;
		Mapflag		map_flags		{};
		cvptr		data			= nullptr;
	};

	struct Buffer {

		BufferType	type	{};
		BufferUsage usage	{};
		Mapflag		flags	{};
		size_t		size	= 0;
		bool		mapped	= false;

		union {
			bufferid gl_handle = 0;
		} handle{};

	};

}