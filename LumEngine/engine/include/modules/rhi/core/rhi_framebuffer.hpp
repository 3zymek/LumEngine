#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct alignas(8) FramebufferHandle {

		FramebufferID id = null_id<FramebufferID>();
		FramebufferID generation = 0;

	};

	enum class FramebufferAttachment : uint8 {
		DepthAttach,
		StencilAttach,
		ColorAttach
	};

	struct FramebufferTextureDescriptor {

		uint32 width = 0;
		uint32 height = 0;

		DataFormat format{};

		bool mipmaps = false;

		FramebufferAttachment attachment{};

	};

	struct Framebuffer {

		FramebufferID handle = 0;

	};

}