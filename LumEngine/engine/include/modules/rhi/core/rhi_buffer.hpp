#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct alignas(8) BufferHandle {
		
		BufferID id = max_val<BufferID>();
		BufferID generation = 0;

	};

	struct BufferDescriptor {
		BufferUsage	bufferUsage	= BufferUsage::Static;
		size_t		size			= 0;
		Mapflag		mapFlags		{};
		cvptr		data			= nullptr;
	};

	struct Buffer {

		BufferType	type	{};
		BufferUsage usage	{};
		Mapflag		flags	{};
		size_t		size	= 0;
		bool		mapped	= false;

		union {
			BufferID glHandle = 0;
		} handle{};

	};

}