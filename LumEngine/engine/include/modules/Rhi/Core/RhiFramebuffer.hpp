//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Framebuffer configuration for the Rendering Hardware Interface.
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

    /* @brief Describes the attachments and dimensions used to create a framebuffer object. */
    struct FramebufferCreateInfo {

        /* @brief Color attachments — pairs of binding slot and texture handle. */
        std::vector<std::pair<uint8, TextureHandle>> m_ColorTex;

        /* @brief Optional depth attachment texture. */
        TextureHandle m_DepthTex;

        /* @brief Optional stencil attachment texture. */
        TextureHandle m_StencilTex;

    };

    /* @brief Describes a blit operation between two framebuffers. */
    struct FramebufferBlitDescription {

        /* @brief Source framebuffer to read from. */
        FramebufferHandle m_Source;

        /* @brief Destination framebuffer to write to. */
        FramebufferHandle m_Destination;

        /* @brief Bottom-left corner of the source rectangle. */
        uint32 m_SrcX0 = 0;
        uint32 m_SrcY0 = 0;

        /* @brief Top-right corner of the source rectangle. */
        uint32 m_SrcX1 = 0;
        uint32 m_SrcY1 = 0;

        /* @brief Bottom-left corner of the destination rectangle. */
        uint32 m_DstX0 = 0;
        uint32 m_DstY0 = 0;

        /* @brief Top-right corner of the destination rectangle. */
        uint32 m_DstX1 = 0;
        uint32 m_DstY1 = 0;

        /* @brief Buffers to copy — color, depth, stencil or a combination. */
        Flags<BufferBit> m_CopyMask;

        /* @brief Filtering method applied when source and destination rectangles differ in size. Use Nearest for depth and stencil. */
        SamplerMagFilter m_Filter = SamplerMagFilter::Nearest;

    };

    /* @brief Internal GPU-side framebuffer resource. */
    struct Framebuffer {

        /* @brief Framebuffer object handle. */
        FramebufferID m_Handle = 0;

    };

} // namespace lum::rhi