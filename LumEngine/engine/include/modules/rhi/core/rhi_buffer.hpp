#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct BufferDescriptor {

		// Defines if buffer is static ( data cannot be changed in runtime ) or dynamic.
		BufferUsage	bufferUsage	= BufferUsage::Static;

		// Size of data that's assigned.
		usize size = 0;

		// Flags defines what operations can be done on a buffer and which not.
		Flags<Mapflag>	mapFlags{};

		// Pointer to data.
		cvptr data = nullptr;
	};

	struct Buffer {

		BufferType		type	{};
		BufferUsage		usage	{};
		Flags<Mapflag>	flags	{};
		usize			size	= 0;
		bool			bMapped	= false;

		union {
			BufferID glHandle = 0;
		} handle{};

	};

}