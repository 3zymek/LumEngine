#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct RHI_BufferHandle {
		
		uint64_t id = std::numeric_limits<uint64_t>::max();
		uint32_t generation = 0;

	};

	using RHIbflag = uint32_t;
	using RHIbhandle = RHI_BufferHandle;

	namespace flags {

		inline constexpr RHIbflag Static	= 1 << 0; // Data cannot be updated in runtime (better performance).
		inline constexpr RHIbflag Dynamic	= 1 << 1; // Data can be updated in runtime (slower performance).

		inline constexpr RHIbflag Map_Persistent			= 1 << 2; // Pointer to the mapped data can be available all the time.
		inline constexpr RHIbflag Map_Write					= 1 << 3; // CPU can only write data through mapping.
		inline constexpr RHIbflag Map_Read					= 1 << 4; // CPU can only read data through mapping.
		inline constexpr RHIbflag Map_Coherent				= 1 << 5; // CPU changes instantly are available for GPU.
		inline constexpr RHIbflag Map_Invalidate_Range		= 1 << 6; // GPU creates new range in buffer, old data is still available.
		inline constexpr RHIbflag Map_Invalidate_Buffer		= 1 << 7; // GPU creates new buffer, destroys the old one.
		inline constexpr RHIbflag Map_Unsynchronized		= 1 << 8; // GPU maps buffer with no backend conditions.

		inline constexpr RHIbflag Vertex_Buffer		= 1 << 9;	// Buffer data type.
		inline constexpr RHIbflag Element_Buffer	= 1 << 10;	// Buffer data type.

	}

	

	struct RHI_BufferDescriptor {
		size_t size;
		RHIbflag flags;
		bool mapped = false;

	};

	struct RHI_Buffer {

		RHIbflag flags = 0;
		size_t size = 0;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

}