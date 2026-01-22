#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct FramebufferHandle {

		framebufferid id = max_val<framebufferid>();
		framebufferid generation = 0;

	};

	enum class FramebufferAttachment : uint8 {
		Depth_Attachment,
		Stencil_Attachment,
		Color_Attachment
	};

	struct FramebufferTextureDescriptor {

		uint32 width = 0;
		uint32 height = 0;

		DataFormat format{};

		bool mipmaps = false;

		FramebufferAttachment attachment{};

	};

	struct FramebufferDescriptor {

	};

	struct Framebuffer {

		framebufferid handle = 0;

	};

}