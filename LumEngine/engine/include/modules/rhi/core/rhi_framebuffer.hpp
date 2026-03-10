//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Framebuffer configuration for the Rendering Hardware Interface.
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"

namespace lum::rhi {

    /* @brief Describes the attachments and dimensions used to create a framebuffer object. */
    struct FFramebufferDescriptor {

        /* @brief Color attachments — pairs of binding slot and texture handle. */
        std::vector<std::pair<uint8, RTextureHandle>> mColorTex;

        /* @brief Optional depth attachment texture. */
        RTextureHandle mDepthTex;

        /* @brief Optional stencil attachment texture. */
        RTextureHandle mStencilTex;

    };

    /* @brief Describes a blit operation between two framebuffers. */
    struct FFramebufferBlitDescriptor {

        /* @brief Source framebuffer to read from. */
        RFramebufferHandle mSource;

        /* @brief Destination framebuffer to write to. */
        RFramebufferHandle mDestination;

        /* @brief Bottom-left corner of the source rectangle. */
        uint32 mSrcX0 = 0;
        uint32 mSrcY0 = 0;

        /* @brief Top-right corner of the source rectangle. */
        uint32 mSrcX1 = 0;
        uint32 mSrcY1 = 0;

        /* @brief Bottom-left corner of the destination rectangle. */
        uint32 mDstX0 = 0;
        uint32 mDstY0 = 0;

        /* @brief Top-right corner of the destination rectangle. */
        uint32 mDstX1 = 0;
        uint32 mDstY1 = 0;

        /* @brief Buffers to copy — color, depth, stencil or a combination. */
        Flags<BufferBit> mCopyMask;

        /* @brief Filtering method applied when source and destination rectangles differ in size. Use Nearest for depth and stencil. */
        SamplerMagFilter mFilter = SamplerMagFilter::Nearest;

    };

    /* @brief Internal GPU-side framebuffer resource. */
    struct FFramebuffer {

        /* @brief Framebuffer object handle. */
        RFramebufferID mHandle = 0;

    };

} // namespace lum::rhi