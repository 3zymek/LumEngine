//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Framebuffer configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	enum class FramebufferAttachment : uint8 {
		DepthAttach,
		StencilAttach,
		ColorAttach
	};

	struct FramebufferTextureDescriptor {

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		DataFormat mFormat{};

		bool bGenerateMipmaps = false;

		FramebufferAttachment mAttachment{};

	};

	struct Framebuffer {

		FramebufferID mHandle = 0;

	};

}