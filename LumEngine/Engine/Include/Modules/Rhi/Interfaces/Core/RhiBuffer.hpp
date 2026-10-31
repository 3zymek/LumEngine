#pragma once

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Flags.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	using BufferID = uint32;
	struct BufferHandle : public BaseHandle<BufferID> {};

	enum class BufferUsage : bitfield {
		Vertex = 1 << 0, // VBO
		Element = 1 << 1, // EBO
		Uniform = 1 << 2, // UBO
		ShaderStorage = 1 << 3,  // SSBO

		TransferSource = 1 << 4, // Source of transfer operations
		TransferDestination = 1 << 5  // Destination of transfer operations
	};
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::BufferUsage );

	using BufferUsageFlags = Flags<BufferUsage>;

	struct BufferCreateInfo {

		BufferUsageFlags m_Usage{};

		usize m_BufferSize = 0;
		usize m_DataSize = 0;
		const void* m_Data = nullptr; // Can be null

	};

	struct BufferInfo {

		BufferUsageFlags m_Usage{};
		usize m_Size = 0;

	};

} // namespace lum::rhi