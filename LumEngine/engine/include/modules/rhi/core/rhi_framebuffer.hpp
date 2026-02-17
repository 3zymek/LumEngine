//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Framebuffer configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	enum class RFramebufferAttachment : uint8 {
		DepthAttach,
		StencilAttach,
		ColorAttach
	};

	struct RFramebufferTextureDescriptor {

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		RDataFormat mFormat{};

		bool bGenerateMipmaps = false;

		RFramebufferAttachment mAttachment{};

	};

	struct RFramebuffer {

		RFramebufferID mHandle = 0;

	};

}