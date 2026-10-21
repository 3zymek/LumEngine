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
		ShaderStorage = 1 << 3  // SSBO
	};

	struct BufferCreateInfo2 {

		Flags<BufferUsage> m_Usage{};

		usize m_BufferSize = 0;
		usize m_DataSize = 0;
		const void* m_Data = nullptr; // Can be null

	};

	struct BufferDescription {

		Flags<BufferUsage> m_Usage{};
		usize m_Size = 0;

	};

} // namespace lum::rhi
namespace lum {

	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS(rhi::BufferUsage);

} // namespace lum