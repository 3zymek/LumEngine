#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	enum class FramebufferAttachment : uint8 {
		DepthAttach,
		StencilAttach,
		ColorAttach
	};

	struct FramebufferTextureDescriptor {

		uint32 width = 0;
		uint32 height = 0;

		DataFormat format{};

		bool bMipmaps = false;

		FramebufferAttachment attachment{};

	};

	struct Framebuffer {

		FramebufferID handle = 0;

	};

}