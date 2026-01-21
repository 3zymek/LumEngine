#pragma once
#include "rhi/rhi_common.hpp"
namespace lum::rhi {

	struct FramebufferHandle {

		framebufferid id = max_val<framebufferid>();
		framebufferid generation = 0;

	};

	struct FramebufferDescriptor {

	};

	struct Framebuffer {

		framebufferid handle = 0;

	};

}