//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Framebuffer configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"

namespace lum::rhi {

	struct FFramebufferDescriptor {

		std::vector<std::pair<uint8, RTextureHandle>> mColorTex;

		RTextureHandle mDepthTex;
		RTextureHandle mStencilTex;

		uint32 mWidth = 0;
		uint32 mHeight = 0;

	};

	struct FFramebuffer {

		RFramebufferID mHandle = 0;

	};

}