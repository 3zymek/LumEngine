#pragma once
#include "Rhi2/RhiCommon.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	using BufferID2 = uint32;
	struct BufferHandle2 : public cstd::BaseHandle<BufferID2> {};

	enum class BufferUsage2 : bitfield {
		Vertex			= 1 << 0, // VBO
		Element			= 1 << 1, // EBO
		Uniform			= 1 << 2, // UBO
		ShaderStorage	= 1 << 3  // SSBO
	};

	struct BufferCreateInfo2 {

		Flags<BufferUsage2> m_Usage{};

		usize m_BufferSize = 0;
		usize m_DataSize = 0;
		const void* m_Data = nullptr; // Can be null

	};

	struct Buffer2 {

		Flags<BufferUsage2> m_Usage{};
		usize m_Size = 0;

	};

} // namespace lum::rhi
namespace lum {
	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::BufferUsage2 );
} // namespace lum