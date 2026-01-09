#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct RHI_BufferHandle {
		
		uint64_t id = std::numeric_limits<uint64_t>::max();
		uint32_t generation = 0;

	};

	enum class RHI_BufferFlags : Bit {
		None = 0,

		// Use
		Static	= 1 << 0, // Data cannot be updated in runtime
		Dynamic = 1 << 1, // Data can be updated in runtime

		// Access
		Map_Persistent	= 1 << 2, // Pointer to the mapped data can be available all the time
		Map_Write		= 1 << 3, // CPU can only write data through mapping
		Map_Read		= 1 << 4, // CPU can only read data through mapping
		Map_Coherent	= 1 << 5, // CPU changes instantly are available for GPU

		// Types
		Vertex	= 1 << 6, // Buffer data type
		Element = 1 << 7, // Buffer data type
	};
	constexpr Bit operator|(RHI_BufferFlags a, RHI_BufferFlags b) noexcept {
		return static_cast<Bit>(a) | static_cast<Bit>(b);
	}
	constexpr Bit operator|(Bit a, RHI_BufferFlags b) noexcept {
		return a | static_cast<Bit>(b);
	}
	constexpr Bit operator|(RHI_BufferFlags a, Bit b) noexcept {
		return static_cast<Bit>(a) | b;
	}

	constexpr Bit operator&(RHI_BufferFlags a, RHI_BufferFlags b) noexcept {
		return static_cast<Bit>(a) & static_cast<Bit>(b);
	}
	constexpr Bit operator&(Bit a, RHI_BufferFlags b) noexcept {
		return a & static_cast<Bit>(b);
	}
	constexpr Bit operator&(RHI_BufferFlags a, Bit b) noexcept {
		return static_cast<Bit>(a) & b;
	}
	

	struct RHI_BufferDescriptor {
		size_t size;
		RHIBFlag flags;

	};

	struct RHI_Buffer {


		size_t size;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

	using RHIBFlag = RHI_BufferFlags;
	using RHIBHandle = RHI_BufferHandle;

}