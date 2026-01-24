#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct alignas(8) FramebufferHandle {

		framebufferid id = max_val<framebufferid>();
		framebufferid generation = 0;

	};

	enum class FramebufferAttachment : uint8 {
		depth_attach,
		stencil_attach,
		color_attach
	};

	struct FramebufferTextureDescriptor {

		uint32 width = 0;
		uint32 height = 0;

		DataFormat format{};

		bool mipmaps = false;

		FramebufferAttachment attachment{};

	};

	struct Framebuffer {

		framebufferid handle = 0;

	};

}